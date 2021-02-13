#! /usr/bin/perl -w

# Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Arnaud Berny

# This file is part of HNCO.

# HNCO is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.

# HNCO is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
# Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with HNCO. If not, see <http://www.gnu.org/licenses/>.

use strict;
use JSON;
use File::Slurp qw(read_file);

my %type_conversion = (
    int         => "int",
    string      => "std::string",
    double      => "double",
    unsigned    => "unsigned"
    );

sub quote
{
    my $s = shift;
    return "\"$s\"";
}

sub quote_in_quote
{
    my $s = shift;
    return "\\\"$s\\\"";
}

#
# Spec parsing
#

my $json = $ARGV[0];

my $obj = from_json(read_file($json));

#
# Global variables
#

my $header = $obj->{files}->{header};
my $source = $obj->{files}->{source};
my $exec = $obj->{files}->{exec};
my $cppdefine = $obj->{code}->{cppdefine};
my $namespace = join "::", @{ $obj->{code}->{namespace} };
my $classname = $obj->{code}->{classname};
my $version = $obj->{version};

my $parameters = $obj->{parameters};
my $flags = $obj->{flags};
my $order = $obj->{order};
my $sections = $obj->{sections};

my %section_hash = ();
my @folded_sections = ();
foreach (@$sections) {
    $section_hash{$_->{id}} = $_;
    if ($_->{fold}) {
        push @folded_sections, $_->{id};
    }
}

# For each section id, list of options (parameter or flag)
my %options = ();

foreach (keys(%$parameters)) {
    if ($parameters->{$_}->{section}) {
        my $id = $parameters->{$_}->{section};
        push @{ $options{$id} }, $_;
    }
}

foreach (keys(%$flags)) {
    if ($flags->{$_}->{section}) {
        my $id = $flags->{$_}->{section};
        push @{ $options{$id} }, $_;
    }
}

generate_header();
generate_source();
generate_completion();

#
# Header
#

sub generate_header
{
    my $file = IO::File->new($header, '>')
        or die "optgen.pl: generate_header: Cannot open $header\n";

    $file->print(
	"#ifndef $cppdefine\n",
	"#define $cppdefine\n\n",
	"#include <iostream>\n",
	"#include <string>\n\n");

    foreach (@{ $obj->{code}->{namespace} }) {
        $file->print("namespace $_ {\n");
    }
    $file->print("\n");

    $file->print(
	"/// Command line options for $exec\n",
	"class $classname {\n\n",
        "  /// Name of the executable\n",
        "  std::string _exec_name;\n\n",
        "  /// Name Version\n",
        "  std::string _version;\n\n");

    foreach (sort(keys(%$parameters))) {
	my $parameter = $parameters->{$_};
        $file->print(
            "  /// ", $parameter->{description}, "\n",
            "  ", $type_conversion{$parameter->{type}}, " _$_;\n",
            "  bool _opt_$_;\n\n");
    }

    foreach (sort(keys(%$flags))) {
	my $flag = $flags->{$_};
        $file->print(
            "  /// $flag->{description}\n",
            "  bool _$_;\n\n");
    }

    $file->print(
        "  /// Print help message\n",
        "  void print_help(std::ostream& stream) const;\n\n");

    foreach (@folded_sections) {
        $file->print(
            "  /// Print help message for section $_\n",
            "  void print_help_$_(std::ostream& stream) const;\n\n");
    }

    $file->print(
        "  /// Print version\n",
        "  void print_version(std::ostream& stream) const;\n\n",
        "public:\n\n",
        "  /// Constructor\n",
        "  $classname(int argc, char *argv[]);\n\n");

    #
    # Access
    #

    foreach (sort(keys(%$parameters))) {
	my $parameter = $parameters->{$_};
	my $type = $parameter->{type};
        $file->print(
            "  /// Get $_\n",
            "  ", $type_conversion{$type}, " get_$_() const { return _$_; }\n\n",
            "  /// Set $_\n",
            "  void set_$_(", $type_conversion{$type}, " x) {\n",
            "    _$_ = x;\n",
            "    _opt_$_ = true;\n",
            "  }\n\n",
            "  /// Get set-flag for $_\n",
            "  bool set_$_() const { return _opt_$_; }\n\n");
    }

    foreach (sort(keys(%$flags))) {
        $file->print(
            "  /// Get $_\n",
            "  bool with_$_() const { return _$_; }\n\n",
            "  /// Set $_\n",
            "  void set_$_() { _$_ = true; }\n\n");
    }

    $file->print(
        "  friend std::ostream& operator<<(std::ostream&, const $classname&);\n",
        "};\n\n",
        "/// Print a header containing the parameter values\n",
        "std::ostream& operator<<(std::ostream& stream, const $classname& options);\n\n");

    foreach (@{ $obj->{code}->{namespace} }) {
        $file->print("}\n");
    }
    $file->print("\n#endif\n");

    $file->close();
}

#
# Source
#

sub generate_source
{
    open(SRC, ">$source");

    print SRC
	"#include <unistd.h>\n",
	"#include <stdlib.h>\n",
	"#include <getopt.h>\n\n",
	"#include \"$header\"\n\n",
        "using namespace $namespace;\n\n";

    generate_source_cons();
    generate_source_help();
    generate_source_additional_section_help();
    generate_source_version();
    generate_source_stream();

    close(SRC);
}

sub generate_source_cons()
{
    print SRC
	"$classname\:\:$classname(int argc, char *argv[]):\n",
	"  _exec_name(argv[0]),\n",
        "  _version(\"$version\"),\n";

    my @plist = map {
	my $parameter = $parameters->{$_};
	my $value = $parameter->{value};
	($parameter->{type} eq "string" ?
	 "  _$_(\"$value\"),\n" :
	 "  _$_($value),\n") .
	 "  _opt_$_(false)";
    } sort(keys(%$parameters));

    my @flist = map { "  _$_(false)" } sort(keys(%$flags));

    print SRC join ",\n", (@plist, @flist);
    print SRC "\n{\n";

    # We start the enum at 256 to avoid any collision with ascii
    # values of characters. For example, the value of 'A' is 65, that
    # of 'Z' is 122. Since an ascii character is represented by 8 bits
    # at most, it seems safe to start at 256.

    print SRC "  enum {\n";
    print SRC "    OPTION_HELP=256,\n";
    if (@folded_sections) {
        print SRC join ",\n", map { "    OPTION_HELP_" . uc($_) } (sort(@folded_sections));
        print SRC ",\n";
    }
    print SRC "    OPTION_VERSION,\n";
    print SRC join ",\n", map { "    OPTION_" . uc($_) } (sort(keys(%$parameters)), sort(keys(%$flags)));
    print SRC "\n";
    print SRC "  };\n";

    print SRC "  const struct option long_options[] = {\n";

    @plist = map {
	my $uppercase = uc($_);
	my $hyphen = $_;
	$hyphen =~ s/_/-/g;
	"    {\"$hyphen\", required_argument, 0, OPTION_$uppercase}"
    } sort(keys(%$parameters));

    @flist = map {
	my $uppercase = uc($_);
	my $hyphen = $_;
	$hyphen =~ s/_/-/g;
	"    {\"$hyphen\", no_argument, 0, OPTION_$uppercase}"
    } (sort(keys(%$flags)), "version", "help", map { "help_$_" } @folded_sections);

    print SRC join ",\n", (@plist, @flist);
    print SRC
	",\n",
	"    {0, no_argument, 0, 0}\n",
	"  };\n";

    @plist = map {
	my $optchar = $parameters->{$_}->{optchar};
	"$optchar:"
    }
    grep { exists($parameters->{$_}->{optchar}); } sort(keys(%$parameters));

    @flist = map {
	my $optchar = $flags->{$_}->{optchar};
	"$optchar"
    }
    grep { exists($flags->{$_}->{optchar}); } sort(keys(%$flags));

    print SRC
	"  const char *short_options = \"" . join("", (@plist, @flist)) . "\";\n",
	"  while (true) {\n",
	"    int option = getopt_long(argc, argv, short_options, long_options, 0);\n",
	"    if (option < 0)\n",
	"      break;\n",
	"    switch (option) {\n";

    foreach (sort(keys(%$parameters))) {

        my $parameter = $parameters->{$_};

	if (exists($parameter->{optchar})) {
	    my $optchar = $parameter->{optchar};
	    print SRC "    case \'$optchar\':\n";
	}

	my $uppercase = uc($_);
	print SRC
	    "    case OPTION_$uppercase:\n",
	    "      set_$_(";

	if (not exists($parameter->{type})) {
	    print "$_\n";
	    exit 1;
	}

        my $type = $parameter->{type};
	if ($type eq "int" or $type eq "bool") {
	    print SRC "atoi(optarg));\n";
	} elsif ($type eq "unsigned") {
	    print SRC "strtoul(optarg, NULL, 0));\n";
	} elsif ($type eq "double") {
	    print SRC "atof(optarg));\n";
	} elsif ($type eq "string") {
	    print SRC "std::string(optarg));\n";
	} else {
	    die "$type: unknown type\n";
	}

	print SRC "      break;\n\n"
    }

    foreach (sort(keys(%$flags))) {
	my $flag = $flags->{$_};
	if (exists($flag->{optchar})) {
	    my $optchar = $flag->{optchar};
	    print SRC "    case \'$optchar\':\n";
	}
	my $uppercase = uc($_);
	print SRC
	    "    case OPTION_$uppercase:\n",
	    "      _$_ = true;\n",
	    "      break;\n\n";
    }

    print SRC
	"    case OPTION_HELP:\n",
	"      print_help(std::cerr);\n",
	"      exit(0);\n\n";

    foreach (@folded_sections) {
        print SRC
            "    case OPTION_HELP_", uc($_), ":\n",
            "      print_help_$_(std::cerr);\n",
            "      exit(0);\n\n";
    }

    print SRC
	"    case OPTION_VERSION:\n",
	"      print_version(std::cerr);\n",
	"      exit(0);\n\n",
	"    default:\n",
        "      std::cerr << \"For more information, please enter: \" << _exec_name << \" --help\" << std::endl;\n",
	"      exit(1);\n",
	"    }\n",
	"  }\n",
	"}\n\n";
}

sub generate_source_help_par
{
    my $key = shift;
    my $par = $parameters->{$key};
    my $desc = $par->{description};
    my $value = $par->{value};
    my $type = $par->{type};

    if ($type eq "string") {
        $value = quote_in_quote($value);
    }

    my $hyphen = $key;
    $hyphen =~ s/_/-/g;

    if (exists($par->{optchar})) {
	my $optchar = $par->{optchar};
	print SRC "  stream << \"  -$optchar, --$hyphen (type $type, default to $value)\" << std::endl;\n";
    } else {
	print SRC "  stream << \"      --$hyphen (type $type, default to $value)\" << std::endl;\n";
    }
    print SRC "  stream << \"          $desc\" << std::endl;\n";

    if (exists($par->{alternatives})) {
	my $alternatives = $par->{alternatives};
	foreach (@$alternatives) {
	    my $v = $_->{value};
	    my $d = $_->{description};
	    print SRC "  stream << \"            $v: $d\" << std::endl;\n";
	}
    }
}

sub generate_source_help_flag
{
    my $key = shift;
    my $flag = $flags->{$key};
    my $desc = $flag->{description};

    my $hyphen = $key;
    $hyphen =~ s/_/-/g;

    if (exists($flag->{optchar})) {
	my $optchar = $flag->{optchar};
	print SRC "  stream << \"  -$optchar, --$hyphen\" << std::endl;\n";
    } else {
	print SRC "  stream << \"      --$hyphen\" << std::endl;\n";
    }
    print SRC "  stream << \"          $desc\" << std::endl;\n";
}

sub generate_source_help_folded_section
{
    my $id = shift;
    my $hyphen = "help-$id";
    $hyphen =~ s/_/-/g;

    print SRC "  stream << ", quote("      --$hyphen"), " << std::endl;\n";
    print SRC "  stream << ", quote("          $section_hash{$id}->{title}"), " << std::endl;\n";
}

sub generate_source_help()
{
    my $classname = $obj->{code}->{classname};

    print SRC
	"void $classname\:\:print_help(std::ostream& stream) const\n",
	"{\n",
        "  stream << ", quote($obj->{description}), " << std::endl << std::endl;\n",
        "  stream << ", quote("usage: "), " << _exec_name << ", quote(" [--help] [--version] [options]"), " << std::endl << std::endl;\n";

    if (not $obj->{sections}) {
	my @list = (keys(%$parameters), keys(%$flags));
	foreach (sort(@list)) {
	    if (exists($parameters->{$_})) {
		generate_source_help_par $_;
	    } else {
		generate_source_help_flag $_;
	    }
	}
    } else {
	foreach (@$order) {
            if (not $section_hash{$_}->{fold}) {
                my $title = $section_hash{$_}->{title};
                print SRC "  stream << ", quote($title), " << std::endl;\n";
                my @list = @{ $options{$_} };
                foreach (sort(@list)) {
                    if (exists($parameters->{$_})) {
                        generate_source_help_par $_;
                    } else {
                        generate_source_help_flag $_;
                    }
                }
                print SRC "  stream << std::endl;\n";
            }
	}
        if (@folded_sections) {
            print SRC "  stream  << ", quote("Additional Sections"), " << std::endl;\n";
            foreach (@folded_sections) {
                generate_source_help_folded_section($_);
            }
        }
    }

    print SRC "}\n\n";
}

sub generate_source_additional_section_help()
{
    foreach (@folded_sections) {
        print SRC
            "void $classname\:\:print_help_$_(std::ostream& stream) const\n",
            "{\n",
            "  stream << ", quote($obj->{description}), " << std::endl << std::endl;\n",
            "  stream << ", quote("usage: "), " << _exec_name << ", quote(" [--help] [--version] [options]"), " << std::endl << std::endl;\n";
        my $title = $section_hash{$_}->{title};
        print SRC "  stream << ", quote($title), " << std::endl;\n";
        foreach (sort(@{ $options{$_} })) {
            if (exists($parameters->{$_})) {
                generate_source_help_par $_;
            } else {
                generate_source_help_flag $_;
            }
        }
        print SRC "  stream << std::endl;\n";
        print SRC "}\n\n";
    }
}

sub generate_source_version()
{
    print SRC
	"void $classname\:\:print_version(std::ostream& stream) const\n",
	"{\n",
        "  stream << _version << std::endl;\n",
        "}\n\n";
}

sub generate_source_stream() {
    print SRC <<EOF;
std::ostream& $namespace\:\:operator<<(std::ostream& stream, const $classname& options)
{
EOF
    foreach(sort(keys(%$parameters))) {
	my $parameter = $parameters->{$_};
	print SRC "  stream << \"# $_ = \" << options._$_ << std::endl;\n";
    }

    foreach(sort(keys(%$flags))) {
	my $flag = $flags->{$_};
	print SRC <<EOF;
  if (options._$_)
    stream << \"# $_\" << std::endl;
EOF
    }

    print SRC <<EOF;
  stream << \"# last_parameter\" << std::endl;
  stream << \"# exec_name = \" << options._exec_name << std::endl;
  stream << \"# version = \" << options._version << std::endl;
  stream << \"# Generated from $json\" << std::endl;
  return stream;
}
EOF
}


#
# Bash completion
#

sub generate_completion
{
    open(COMP, ">$exec.sh") || die "Cannot open $exec.sh\n";;

    my @list = map {
	my $hyphen = $_;
	$hyphen =~ s/_/-/g;
	"--$hyphen"
    } sort(keys(%$parameters)), sort(keys(%$flags), "version", "help", map { "help_$_" } @folded_sections);

    my $options = join " ", @list;

    print COMP <<EOF;
_$exec() 
{
    local cur prev opts
    COMPREPLY=()
    cur=\"\${COMP_WORDS[COMP_CWORD]}\"
    prev=\"\${COMP_WORDS[COMP_CWORD-1]}\"
    opts=\"$options\"

    if [[ \${cur} == -* ]] ; then
        COMPREPLY=( \$(compgen -W \"\${opts}\" -- \${cur}) )
        return 0
    fi
}
complete -F _$exec $exec
EOF

    close(COMP);
}
