import os

if __name__ == "__main__": 
	if(not os.path.exists("../../Dependencies/marmalade/freetype")):
		os.system("git clone https://github.com/marmalade/Freetype.git ../../Dependencies/marmalade/freetype")