#! /usr/bin/perl -w

# Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024 Arnaud Berny

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
    int      => "int",
    string   => "std::string",
    double   => "double",
    unsigned => "unsigned",
    bool     => "bool"
    );

#
# Spec parsing
#

my $json = $ARGV[0];

my $obj = from_json(read_file($json));

#
# Global variables
#

my $code        = $obj->{code};
my $description = $obj->{description};
my $flags       = $obj->{flags};
my $order       = $obj->{order};
my $parameters  = $obj->{parameters};
my $sections    = $obj->{sections};
my $version     = $obj->{version};
my $files       = $obj->{files};

my $exec        = $files->{exec};
my $header      = $files->{header};
my $source      = $files->{source};

my $classname   = $code->{classname};
my $namespace   = join "::", @{ $code->{namespace} };

my @folded_sections = ();

my %section_by_id   = ();

# For each section id, list of options (parameter or flag)
my %options_by_id   = ();

# requirement: section ids are unique
foreach (@$sections) {
    my $id = $_->{id};
    if (exists($section_by_id{$id})) {
        die "optgen.pl: already used section id: $id\n";
    }
    $section_by_id{$id} = $_;
    if ($_->{fold}) {
        push @folded_sections, $id;
    }
}

# requirement: @$order and keys(%section_by_id) are identical
foreach (@$order) {
    unless (exists($section_by_id{$_})) {
        die "optgen.pl: order: unkown section id: ", $_, "\n";
    }
}

if (@$order != keys(%section_by_id)) {
    die "optgen.pl: array order is incomplete\n";
}

# Group parameters and flags by section
foreach (keys(%$parameters)) {
    my $parameter = $parameters->{$_};
    if (exists($parameter->{section})) {
        my $id = $parameter->{section};
        push @{ $options_by_id{$id} }, $_;
    }
}

foreach (keys(%$flags)) {
    my $flag = $flags->{$_};
    if (exists($flag->{section})) {
        my $id = $flag->{section};
        push @{ $options_by_id{$id} }, $_;
    }
}

# Check for empty sections
foreach (@$order) {
    if (exists($options_by_id{$_})) {
        next;
    } else {
        print "optgen.pl: $_ is an empty section\n";
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

    my $cppdefine = $code->{cppdefine};
    $file->print(
	"#ifndef $cppdefine\n",
	"#define $cppdefine\n\n",
	"#include <iostream>\n",
	"#include <string>\n\n");

    foreach (@{ $code->{namespace} }) {
        $file->print("namespace $_ {\n");
    }
    $file->print("\n");

    $file->print(
	"/// Command line options for $exec\n",
	"class $classname {\n\n",
        "  /// Name of the executable\n",
        "  std::string _exec_name;\n\n",
        "  /// Name Version\n",
        "  std::string _version = \"$version\";\n\n");

    # We start the enum at 256 to avoid any collision with ascii
    # values of characters. For example, the value of 'A' is 65, that
    # of 'Z' is 122. Since an ascii character is represented by 8 bits
    # at most, it seems safe to start at 256.

    $file->print(
        "  enum {\n",
        "    OPTION_HELP=256,\n");

    if (@folded_sections) {
        $file->print(
            join(",\n", map { "    OPTION_HELP_" . uc($_) } (sort(@folded_sections))),
            ",\n");
    }

    $file->print(
        "    OPTION_VERSION,\n",
        join(",\n", map { "    OPTION_" . uc($_) } (sort(keys(%$parameters)), sort(keys(%$flags)))),
        "\n",
        "  };\n\n");

    #
    # Parameters
    #

    foreach (sort(keys(%$parameters))) {
	my $parameter = $parameters->{$_};
        $file->print("  /// ", $parameter->{description}, "\n");
        my $type = $type_conversion{$parameter->{type}};
        if (exists($parameter->{default})) {
            if ($parameter->{type} eq "string") {
                $file->print(qq(  $type _$_ = "$parameter->{default}";\n));
            } elsif ($parameter->{type} eq "bool") {
                $file->print("  $type _$_ = ", ($parameter->{default} ? "true" : "false"), ";\n");
            } else {
                $file->print("  $type _$_ = ", $parameter->{default}, ";\n");
            }
        } else {
            $file->print("  $type _$_;\n");
        }
        $file->print("  bool _with_$_ = false;\n\n");
    }

    #
    # Flags
    #

    foreach (sort(keys(%$flags))) {
	my $flag = $flags->{$_};
        $file->print(
            "  /// $flag->{description}\n",
            "  bool _$_ = false;\n\n");
    }

    #
    # Help member functions
    #

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
        "  /// Default constructor\n",
	"  $classname(): _exec_name(\"$exec\") {}\n\n",
        "  /// Constructor\n",
        "  $classname(int argc, char *argv[], bool ignore_bad_options = false);\n\n");

    #
    # Access
    #

    foreach (sort(keys(%$parameters))) {
	my $parameter = $parameters->{$_};
        my $type = $type_conversion{$parameter->{type}};
        $file->print("  /// Get the value of $_\n");
        if (exists($parameter->{default})) {
            my $line = qq/  $type get_$_() const { return _$_; }/;
            $file->print($line);
            $file->print("\n\n");
        } else {
            my @lines = (
                qq/  $type get_$_() const {/,
                qq/    if (_with_$_)/,
                qq/      return _$_;/,
                qq/    else/,
                qq/      throw std::runtime_error("$classname\:\:get_$_: Parameter $_ has no default value and has not been set");/,
                qq/    }/,
                );
            foreach (@lines) {
                $file->print($_, "\n");
            }
            $file->print("\n");
        }
        $file->print(
            "  /// With parameter $_\n",
            "  bool with_$_() const { return _with_$_; }\n\n");
    }

    foreach (sort(keys(%$flags))) {
        $file->print(
            "  /// With the flag $_\n",
            "  bool with_$_() const { return _$_; }\n\n");
    }

    $file->print(
        "  friend std::ostream& operator<<(std::ostream&, const $classname&);\n",
        "};\n\n",
        "/// Print a header containing the parameter values\n",
        "std::ostream& operator<<(std::ostream& stream, const $classname& options);\n\n");

    foreach (@{ $code->{namespace} }) {
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
	"#include <cstdlib>\n",
	"#include <getopt.h>\n\n",
	"#include \"$header\"\n\n",
        "using namespace $namespace;\n\n";

    generate_source_constructor();
    generate_source_help();
    generate_source_additional_section_help();
    generate_source_version();
    generate_source_stream();

    close(SRC);
}

sub generate_source_constructor()
{
    print SRC
	"$classname\:\:$classname(int argc, char *argv[], bool ignore_bad_options):\n",
	"  _exec_name(argv[0])\n",
        "{\n";

    print SRC "  const struct option long_options[] = {\n";

    my @plist = map {
	my $uppercase = uc($_);
	my $hyphen = $_;
	$hyphen =~ s/_/-/g;
	"    {\"$hyphen\", required_argument, 0, OPTION_$uppercase}"
    } sort(keys(%$parameters));

    my @flist = map {
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
	"  optind = 0;\n",
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
            "      _with_$_ = true;\n",
            "      _$_ = ";

        my $type = $parameter->{type};
	if ($type eq "int") {
	    print SRC "std::atoi(optarg);\n";
	} elsif ($type eq "bool") {
	    print SRC "(std::string(optarg) == \"true\") ? true : false;\n";
	} elsif ($type eq "unsigned") {
	    print SRC "std::strtoul(optarg, NULL, 0);\n";
	} elsif ($type eq "double") {
	    print SRC "std::atof(optarg);\n";
	} elsif ($type eq "string") {
	    print SRC "std::string(optarg);\n";
	} else {
	    die "optgen.pl: $type: unknown type\n";
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
        "      if (!ignore_bad_options) {\n",
        "        std::cerr << \"For more information, please enter: \" << _exec_name << \" --help\" << std::endl;\n",
	"        exit(1);\n",
	"      }\n",
	"    }\n",
	"  }\n",
	"}\n\n";

}

sub generate_source_help_par
{
    my $key = shift;

    my $parameter = $parameters->{$key};
    my $desc      = $parameter->{description};
    my $type      = $parameter->{type};

    my $hyphen = $key;
    $hyphen =~ s/_/-/g;

    my $default = "no default";
    if (exists($parameter->{default})) {
        if ($type eq "string") {
            $default = $parameter->{default};
            $default = qq(\\"$default\\");
        } elsif ($type eq "bool") {
            $default = ($parameter->{default} ? "true" : "false");
        } else {
            $default = "$parameter->{default}";
        }
        $default = "default to $default";
    }

    if (exists($parameter->{optchar})) {
	my $optchar = $parameter->{optchar};
	print SRC "  stream << \"  -$optchar, --$hyphen (type $type, $default)\" << std::endl;\n";
    } else {
	print SRC "  stream << \"      --$hyphen (type $type, $default)\" << std::endl;\n";
    }
    print SRC "  stream << \"          $desc\" << std::endl;\n";

    if (exists($parameter->{values})) {
	my $values = $parameter->{values};
	foreach (@$values) {
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

    print SRC
        qq(  stream << "      --$hyphen" << std::endl;\n),
        qq(  stream << "          $section_by_id{$id}->{title}" << std::endl;\n);
}

sub generate_source_help()
{
    my $classname = $code->{classname};

    print SRC
	"void $classname\:\:print_help(std::ostream& stream) const\n",
	"{\n",
        qq(  stream << "$description" << std::endl << std::endl;\n),
        qq(  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;\n);

    if ($sections) {
	foreach (@$order) {
            unless ($section_by_id{$_}->{fold}) {
                my $title = $section_by_id{$_}->{title};
                print SRC qq(  stream << "$title" << std::endl;\n);
                if (exists($options_by_id{$_})) {
                    my @list = @{ $options_by_id{$_} };
                    foreach (sort(@list)) {
                        if (exists($parameters->{$_})) {
                            generate_source_help_par $_;
                        } else {
                            generate_source_help_flag $_;
                        }
                    }
                }
                print SRC "  stream << std::endl;\n";
            }
	}
        if (@folded_sections) {
            print SRC qq(  stream << "Additional Sections" << std::endl;\n);
            foreach (@folded_sections) {
                generate_source_help_folded_section($_);
            }
        }
    } else {
	my @list = (keys(%$parameters), keys(%$flags));
	foreach (sort(@list)) {
	    if (exists($parameters->{$_})) {
		generate_source_help_par $_;
	    } else {
		generate_source_help_flag $_;
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
            qq(  stream << "$description" << std::endl << std::endl;\n),
            qq(  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;\n);
        my $title = $section_by_id{$_}->{title};
        print SRC qq(  stream << "$title" << std::endl;\n);
        if (exists($options_by_id{$_})) {
            foreach (sort(@{ $options_by_id{$_} })) {
                if (exists($parameters->{$_})) {
                    generate_source_help_par $_;
                } else {
                    generate_source_help_flag $_;
                }
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

sub generate_source_stream()
{
    my @lines = ();

    push @lines, "std::ostream& $namespace\:\:operator<<(std::ostream& stream, const $classname& options)";
    push @lines, "{";

    foreach (sort(keys(%$parameters))) {
	my $parameter = $parameters->{$_};
        my $type = $parameter->{type};
        if (exists($parameter->{default})) {
            if ($type eq "string") {
                push @lines, qq(  stream << "# $_ = \\"" << options._$_ << "\\"" << std::endl;);
            } elsif ($type eq "bool") {
                push @lines, qq(  stream << "# $_ = " << (options._$_ ? "true" : "false") << std::endl;);
            } else {
                push @lines, qq(  stream << "# $_ = " << options._$_ << std::endl;);
            }
        } else {
            push @lines, "  if (options._with_$_)";
            if ($type eq "string") {
                push @lines, qq(    stream << "# $_ = \\"" << options._$_ << "\\"" << std::endl;);
            } elsif ($type eq "bool") {
                push @lines, qq(    stream << "# $_ = " << (options._$_ ? "true" : "false") << std::endl;);
            } else {
                push @lines, qq(    stream << "# $_ = " << options._$_ << std::endl;);
            }
        }
    }

    foreach (sort(keys(%$flags))) {
        push @lines, "  if (options._$_)";
        push @lines, "    stream << \"# $_ \" << std::endl;";
    }

    push @lines, qq(  stream << "# last_parameter" << std::endl;);
    push @lines, qq(  stream << "# exec_name = " << options._exec_name << std::endl;);
    push @lines, qq(  stream << "# version = " << options._version << std::endl;);
    push @lines, qq(  stream << "# Generated from $json" << std::endl;);
    push @lines, "  return stream;";
    push @lines, "}";

    foreach (@lines) {
        print SRC $_, "\n";
    }
}


#
# Bash completion
#

sub generate_completion
{
    open(COMP, ">$exec.bash")
        or die "optgen.pl: cannot open $exec.bash\n";

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
