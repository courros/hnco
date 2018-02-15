#! /usr/bin/perl -w

# Copyright (C) 2016, 2017, 2018 Arnaud Berny

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

my %type_conversion = (
    int         => "int",
    string      => "std::string",
    double      => "double",
    unsigned    => "unsigned"
    );

#
# Spec parsing
#

my $file = $ARGV[0];
open(FILE, $file) || die "Cannot open $file\n";

my $json = "";
while (<FILE>) {
    $json .= $_;
}

my $obj = from_json($json);
my $parameters = $obj->{"parameters"};
my $flags = $obj->{"flags"};
my $tab_order = $obj->{"tab_order"};


#
# Header
#

sub generate_header {

    my $name = $obj->{"name"};

    my $header = join "-", @$name;
    $header .=  "-options.hh";

    open(HEADER, ">$header");

    my $symbol = uc(join "_", @$name);
    $symbol .= "_OPTIONS_H";

    print HEADER
	"#ifndef $symbol\n",
	"#define $symbol\n\n",
	"#include <iostream>\n",
	"#include <string>\n\n",
	"/// Command line options\n",
	"class Options {\n\n";

    print HEADER <<EOF;
  /// Name of the executable
  std::string _exec_name;

  /// Version
  std::string _version;

EOF

    foreach (sort(keys(%$parameters))) {
	my $parameter = $parameters->{$_};
        print HEADER
            "  /// ", $parameter->{"description"}, "\n",
            "  ", $type_conversion{$parameter->{"type"}}, " _$_;\n",
            "  bool _opt_$_;\n\n";
}

    foreach (sort(keys(%$flags))) {
	my $flag = $flags->{$_};
	print HEADER <<EOF;
  /// $flag->{"description"}
  bool _$_;

EOF
    }

    print HEADER <<EOF;
  /// Print help message
  void print_help(std::ostream& stream) const;

  /// Print version
  void print_version(std::ostream& stream) const;

public:

  /// Constructor
  Options(int argc, char *argv[]);

EOF


#
# Access
#
    foreach (sort(keys(%$parameters))) {
	my $parameter = $parameters->{$_};
	my $type = $parameter->{"type"};
        print HEADER
            "  /// Get $_\n",
            "  ", $type_conversion{$type}, " get_$_() const { return _$_; }\n\n",
            "  /// Set $_\n",
            "  void set_$_(", $type_conversion{$type}, " x) {\n",
            "    _$_ = x;\n",
            "    _opt_$_ = true;\n",
            "  }\n\n",
            "  /// Get set-flag for $_\n",
            "  bool set_$_() const { return _opt_$_; }\n\n";
}

    foreach (sort(keys(%$flags))) {
	print HEADER <<EOF;
  /// Get $_
  bool with_$_() const { return _$_; }

  /// Set $_
  void set_$_() { _$_ = true; }
 
EOF
    }

    print HEADER <<EOF;
  /// Print a header containing the parameter values
  friend std::ostream& operator<<(std::ostream&, const Options&);
};


#endif
EOF
    close(HEADER);
}


#
# Source
#

sub generate_source {

    my $name = $obj->{"name"};
    my $prefix = join "-", @$name;
    my $header = $prefix . "-options.hh";
    my $source = $prefix . "-options.cc";

    open(SRC, ">$source");

    print SRC
	"#include <unistd.h>\n",
	"#include <stdlib.h>\n",
	"#include <getopt.h>\n\n",
	"#include \"$header\"\n\n",
        "using namespace std;\n\n";

    generate_source_cons();
    generate_source_help();
    generate_source_version();
    generate_source_stream();

    close(SRC);
}


sub generate_source_cons() {

    my $version = $obj->{"version"};

    print SRC
	"Options\:\:Options(int argc, char *argv[]):\n",
	"  _exec_name(argv[0]),\n",
        "  _version(\"$version\"),\n";

    my @plist = map {
	my $parameter = $parameters->{$_};
	my $value = $parameter->{"value"};
	($parameter->{"type"} eq "string" ?
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
    } (sort(keys(%$flags)), "help", "version");

    print SRC join ",\n", (@plist, @flist);
    print SRC
	",\n",
	"    {0, no_argument, 0, 0}\n",
	"  };\n";

    @plist = map {
	my $optchar = $parameters->{$_}->{"optchar"};
	"$optchar:"
    }
    grep { exists($parameters->{$_}->{"optchar"}); } sort(keys(%$parameters));

    @flist = map {
	my $optchar = $flags->{$_}->{"optchar"};
	"$optchar"
    }
    grep { exists($flags->{$_}->{"optchar"}); } sort(keys(%$flags));

    print SRC
	"  const char *short_options = \"" . join("", (@plist, @flist)) . "\";\n",
	"  while (true) {\n",
	"    int option = getopt_long(argc, argv, short_options, long_options, 0);\n",
	"    if (option < 0)\n",
	"      break;\n",
	"    switch (option) {\n";

    foreach (sort(keys(%$parameters))) {

        my $parameter = $parameters->{$_};

	if (exists($parameter->{"optchar"})) {
	    my $optchar = $parameter->{"optchar"};
	    print SRC "    case \'$optchar\':\n";
	}

	my $uppercase = uc($_);
	print SRC
	    "    case OPTION_$uppercase:\n",
	    "      set_$_(";

	if (not exists($parameter->{"type"})) {
	    print "$_\n";
	    exit 1;
	}

        my $type = $parameter->{"type"};
	if ($type eq "int" or
	    $type eq "bool") {
	    print SRC "atoi(optarg));\n";
	}
	elsif ($type eq "unsigned") {
	    print SRC "strtoul(optarg, NULL, 0));\n";
	}
	elsif ($type eq "double") {
	    print SRC "atof(optarg));\n";
	}
	elsif ($type eq "string") {
	    print SRC "string(optarg));\n";
	}
	else {
	    die "$type: unknown type\n";
	}

	print SRC "      break;\n\n"
    }

    foreach (sort(keys(%$flags))) {
	my $flag = $flags->{$_};
	if (exists($flag->{"optchar"})) {
	    my $optchar = $flag->{"optchar"};
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
	"      print_help(cerr);\n",
	"      exit(0);\n\n",
	"    case OPTION_VERSION:\n",
	"      print_version(cerr);\n",
	"      exit(0);\n\n",
	"    default:\n",
        "      cerr << \"For more information, please enter: \" << _exec_name << \" --help\" << endl;\n",
	"      exit(1);\n",
	"    }\n",
	"  }\n",
	"}\n\n";
}


sub quote
{
    my $s = shift;
    return "\\\"$s\\\"";
}

sub generate_source_help_par {

    my $key = shift;
    my $par = $parameters->{$key};
    my $desc = $par->{"description"};
    my $value = $par->{"value"};
    my $type = $par->{"type"};

    if ($type eq "string") {
        $value = quote($value);
    }

    my $hyphen = $key;
    $hyphen =~ s/_/-/g;

    if (exists($par->{"optchar"})) {
	my $optchar = $par->{"optchar"};
	print SRC "  stream << \"  -$optchar, --$hyphen (type $type, default to $value)\" << endl;\n";
    }
    else {
	print SRC "  stream << \"      --$hyphen (type $type, default to $value)\" << endl;\n";
    }
    print SRC "  stream << \"          $desc\" << endl;\n";

    if (exists($par->{"alternatives"})) {
	my $alternatives = $par->{"alternatives"};
	foreach (@$alternatives) {
	    my $v = $_->{"value"};
	    my $d = $_->{"description"};
	    print SRC "  stream << \"            $v: $d\" << endl;\n";
	}
    }

}


sub generate_source_help_flag {

    my $key = shift;
    my $flag = $flags->{$key};
    my $desc = $flag->{"description"};

    my $hyphen = $key;
    $hyphen =~ s/_/-/g;

    if (exists($flag->{"optchar"})) {
	my $optchar = $flag->{"optchar"};
	print SRC "  stream << \"  -$optchar, --$hyphen\" << endl;\n";
    }
    else {
	print SRC "  stream << \"      --$hyphen\" << endl;\n";
    }
    print SRC "  stream << \"          $desc\" << endl;\n";

}


sub generate_source_help() {

    print SRC
	"void Options\:\:print_help(ostream& stream) const\n",
	"{\n";

    my $what = $obj->{"description"};

    print SRC
	"  stream << \"$what\" << endl << endl;\n",
	"  stream << \"usage: \" << _exec_name << \" [--help] [--version] [options]\" << endl << endl;\n";

    if (exists($obj->{"nocategory"}) && $obj->{"nocategory"}) {
	my @list = (keys(%$parameters), keys(%$flags));
	foreach (sort(@list)) {
	    if (exists($parameters->{$_})) {
		generate_source_help_par $_;
	    }
	    else {
		generate_source_help_flag $_;
	    }
	}
    }
    else {

	my %category = ();

	foreach (keys(%$parameters)) {
	    my $cat = $parameters->{$_}->{"category"};
	    push @{ $category{$cat} }, $_;
	}

	foreach (keys(%$flags)) {
	    my $cat = $flags->{$_}->{"category"};
	    push @{ $category{$cat} }, $_;
	}

	foreach (@$tab_order) {
	    print SRC "  stream << \"$_:\" << endl;\n";
	    my @list = @{ $category{$_} };
	    foreach (sort(@list)) {
		if (exists($parameters->{$_})) {
		    generate_source_help_par $_;
		}
		else {
		    generate_source_help_flag $_;
		}
	    }

	    print SRC "  stream << endl;\n";

	}

    }

    print SRC "}\n\n";

}


sub generate_source_version() {

    print SRC
	"void Options\:\:print_version(ostream& stream) const\n",
	"{\n",
        "  stream << _version << endl;\n",
        "}\n\n";

}


sub generate_source_stream() {
    print SRC <<EOF;
ostream& operator<<(ostream& stream, const Options& options)
{
EOF
    foreach(sort(keys(%$parameters))) {
	my $parameter = $parameters->{$_};
	print SRC "  stream << \"# $_ = \" << options._$_ << endl;\n";
    }

    foreach(sort(keys(%$flags))) {
	my $flag = $flags->{$_};
	print SRC <<EOF;
  if (options._$_)
    stream << \"# $_\" << endl;
EOF
    }

    print SRC <<EOF;
  stream << \"# last_parameter\" << endl;
  stream << \"# exec_name = \" << options._exec_name << endl;
  stream << \"# version = \" << options._version << endl;
  stream << \"# Generated from $file\" << endl;
  return stream;
}
EOF
}


#
# Bash completion
#

sub generate_completion {

    my $name = $obj->{"name"};
    my $binary = join "-", @$name;
    my $function = join "_", @$name;

    open(COMP, ">$binary.sh");

    my @list = map {
	$_ =~ s/_/-/g;
	"--$_"
    } sort(keys(%$parameters)), sort(keys(%$flags));

    my $options = join " ", @list, "--help", "--version";

    print COMP <<EOF;
_$function() 
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
complete -F _$function $binary
EOF

    close(COMP);
}


generate_header();
generate_source();
generate_completion();

exit;
