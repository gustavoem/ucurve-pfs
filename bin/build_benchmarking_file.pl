#!/usr/bin/perl -w 

#
# build_benchmarking_file.pl : script to generate the benchmarking
#                              program file taking into account the
#                              current set of available cost functions.
#
#    This file is part of the featsel program
#    Copyright (C) 2016  Marcelo S. Reis
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

use strict;

my $date_string = localtime ();

#------------------------------------------------------------------------------#
# First, we load the lists of cost functions.
#
my $list_of_cost_functions_file = "ListOfCostFunctions.txt";

open (LIST, $list_of_cost_functions_file) or 
  die "Error: could not open $list_of_cost_functions_file for reading!\n";

my @list_of_function_codes;
my @list_of_function_class_names;
my @list_of_function_file_types;

while (<LIST>)
{
  chomp $_;
  # $1 == code, $2 == class name, $3 == file type
  #
  if ($_ =~ /^\s*(\w+)\s+(\w+)\s+(\w+)\s*$/) 
  {  
    push @list_of_function_codes, $1;
    push @list_of_function_class_names, $2;
    push @list_of_function_file_types, $3;
  }
  elsif ($_ =~ /^\s*$/)
  {
    # Blank line; do nothing.
  }
  else
  {
    die "Error: could not parse line of $list_of_cost_functions_file!\n";
  }
}

close (LIST);

#------------------------------------------------------------------------------#

# Now we update run_benchmarking.pl
#
open (IN, "bin/run_benchmarking.tpl")
  or die "Could not open 'bin/run_benchmarking.tpl'!\n";
open (OUT, ">bin/run_benchmarking.pl")
  or die "Could not open 'bin/run_benchmarking.pl'!\n";

printf OUT "#!/usr/bin/perl -w\n\n";

printf OUT "# run_benchmarking.cpp automatically generated by $0\n" .
           "# in $date_string.\n\n";

while (<IN>)
{
  if ($_ =~ /\%template_use\%/)
  {
    foreach my $name (@list_of_function_class_names)
    {
      print OUT "use $name;\n";
    }
  }
  elsif ($_ =~ /\%template_if\%/)
  {
    foreach my $index (0..$#list_of_function_class_names)
    {
      printf OUT "      elsif (\$cost_function eq '%s')\n" .
                 "      {\n" .
                 "        %s::random_%s\_instance\n" .
                 "        (\$i, \$MAX_ELEM_VALUE, \$INPUT_DIR, \$_);\n" .
                 "      }\n", 
                 $list_of_function_codes[$index],
                 $list_of_function_class_names[$index],
                 $list_of_function_codes[$index];
    }
  }
  else
  {
    print OUT $_;
  }
}

close (IN);
close (OUT);

#------------------------------------------------------------------------------#

# End of execution.
#
exit 0;   