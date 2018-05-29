# Python script to calculate neutron detector efficiency
# from data file
# Ron Malone January 2016

import math
import os.path

print
print "Efficiency value retrieval"

fname = input("What is the path to data file from here? Please enclose in quotes: ")

#Ask user to input relevant parameters
energy = input("What is the neutron energy (in MeV)?  ")


infile = open(fname,"r")

# Calculate energy steps and starting point
# based on first two lines of file
myline = infile.next().strip().split() # get first line

if myline[0] == '#': # some lines in data file "commented" with hashes
    # need to skip to second element for energy
    en1 = float(myline[1])
else: # first element is energy
    en1 = float(myline[0])

myline = infile.next().strip().split() # get second line

if myline[0] == '#': # some lines in data file "commented" with hashes
    # need to skip to second element for energy
    en2 = float(myline[1])
else: # first element is energy
    en2 = float(myline[0])

eStep = en2 - en1 # calculate energy step in file
    
# Calculate energy indices for interpolation
en = float(energy) # convert string to float
# subtract first point, divide by step
enIndex = math.floor( (energy - en1) / eStep)

infile.seek(0) # return to top of file
# Skip lines to first  energy
for i in range(0,int(enIndex)):
    infile.next()
    
mystring = infile.next().strip().split() # get line
en1 = float(mystring[0]) # get en 1
eff1 = float(mystring[1]) # get eff 1

mystring = infile.next().strip().split() # get next line
en2 = float(mystring[0]) # get en 2
eff2 = float(mystring[1]) # get eff 2

# interpolate
diff = en - en1 # difference between energy and index

# slope * diff + eff1
eff = ( eff2 - eff1 ) / eStep * diff + eff1

    
print " Neutron energy: ", energy
print " Efficiency: ",format(eff,'.4f') # truncate after 4 places





