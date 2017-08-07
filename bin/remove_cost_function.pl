#!/usr/bin/perl -w 

#
# remove_cost_function.pl : program to remove a cost function from the featsel
#                           framework, including its class, its respective
#                           test namespace and modifications in featsel.cpp,
#                           featselTest.cpp and in the Makefile.
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

# Process the program arguments
#
(@ARGV != 2) and 
  die "Syntax: $0 cost_function_code cost_function_class_name\n";

my $code;
($ARGV[0] =~ /(^\w+$)/) and $code = $1 or
  die "Error: cost function code must be alphanumeric!\n";

my $class_name;
($ARGV[1] =~ /(^\w+$)/) and $class_name = $1 or
  die "Error: cost function class name must be alphanumeric!\n";

#------------------------------------------------------------------------------#

# First, we load the list of cost functions, in order to remove one that
# does not exists.
#
print "Loading the lists of algorithms and cost functions...";

my $list_of_cost_functions_file = "ListOfCostFunctions.txt";
my $list_of_algorithms_file = "ListOfAlgorithms.txt";

my $cost_function_is_in_the_list = 0;

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
    if  (($code eq $1) &&
         ($class_name eq $2))
    {
      $cost_function_is_in_the_list = 1;
    }
    else
    {
      push @list_of_function_codes, $1;
      push @list_of_function_class_names, $2;
      push @list_of_function_file_types, $3;
    } 
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

open (LIST, $list_of_algorithms_file) or 
  die "Error: could not open $list_of_algorithms_file for reading!\n";

my @list_of_algorithm_codes;
my @list_of_algorithm_class_names;

while (<LIST>)
{
  chomp $_;
  if ($_ =~ /^\s*(\w+)\s+(\w+)\s*$/) # $1 == code, $2 == class name
  {  
    push @list_of_algorithm_codes, $1;
    push @list_of_algorithm_class_names, $2; 
  }
  elsif ($_ =~ /^\s*$/)
  {
    # Blank line; do nothing.
  }
  else
  {
    die "Error: could not parse line of $list_of_algorithms_file!\n";
  }
}

close (LIST);


(! $cost_function_is_in_the_list) and
  die "Error: the pair <" . $code . "," . $class_name . 
  "> is not in the list!\n";

print " [done]\n";

#------------------------------------------------------------------------------#

# Code and class name are ok; hence, we can update the list of cost functions.
#

print "Updating the list of cost functions...";

open (LIST, ">$list_of_cost_functions_file") or
  die "Error: could not open $list_of_cost_functions_file for writing!\n";

foreach my $index (0..$#list_of_function_codes)
{
  printf LIST "%20s %20s %20s\n", $list_of_function_codes[$index], 
                            $list_of_function_class_names[$index],
                            $list_of_function_file_types[$index];
}

close (LIST);

print " [done]\n";

#------------------------------------------------------------------------------#

# Removing the class file, test namespace file, and instance generator.
#
print "Removing class file, test namespace file, and instance generator...";

system ("rm src/functions/" . $class_name . ".*");

system ("rm test/functions/" . $class_name . "Test.*");

system ("rm lib/" . $class_name . ".pm");

print " [done]\n";


#------------------------------------------------------------------------------#

# Finally, we create a new up-to-date Makefile and update main files.
#

print "Updating Makefile file...";

open (TEMPLATE, "Makefile.tpl") or
  die "Error: could not open Makefile template file!\n";
open (OUT, ">Makefile") or
  die "Error: could not open Makefile output file!\n";

printf OUT "# Makefile automatically generated by $0\n" .
           "# in $date_string.\n\n";

while (<TEMPLATE>)
{
  if ($_ =~ /\%template_class\%/)
  {
    foreach my $index (0..$#list_of_function_codes)
    {
      printf OUT "      src/functions/%s.o \\\n",
      $list_of_function_class_names[$index];
    }
    foreach my $index (0..$#list_of_algorithm_codes)
    {
      printf OUT "      src/algorithms/%s.o \\\n",
      $list_of_algorithm_class_names[$index];
    }
  }
  elsif ($_ =~ /\%template_test\%/)
  {
    foreach my $index (0..$#list_of_function_codes)
    {
      printf OUT "      test/functions/%s" . "Test.o \\\n",
        $list_of_function_class_names[$index];
    }
    foreach my $index (0..$#list_of_algorithm_codes)
    {
      printf OUT "      test/algorithms/%s" . "Test.o \\\n",
        $list_of_algorithm_class_names[$index];
    }
  }
  else
  {
    print OUT $_;
  }
}

close (TEMPLATE);
close (OUT);

print " [done]\n";

print "Updating main, test and benchmarking files...";

system ("bin/build_featsel_main_file.pl");
system ("bin/build_featselTest_main_file.pl");
system ("bin/build_benchmarking_file.pl");

system ("make clean 1> /dev/null");
system ("make 1> /dev/null");

print " [done]\n";

#------------------------------------------------------------------------------#

# End of execution.
#

print "End of execution\n";

exit 0;   