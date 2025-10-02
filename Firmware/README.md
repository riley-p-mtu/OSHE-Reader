# Current Design
Using [Minizip](https://github.com/richgel999/miniz) to try to unzip and extract files/info from the epub, and [Expat](https://github.com/libexpat/libexpat) to parse the XML. The Minizip GitHub page includes examples for different functions they offer, and you can read through the miniz.h file to also gain knowledge on using the functions. Expat has some examples, although [this](https://libexpat.github.io/doc/getting-started/) website has some better documentation. The current reason for using Expat is that it might be a better choice for limited memory on the ESP32. For both libraries, searching the functions directly in their repositories can provide context on how to use them. Also included in the epub for MobyDick, which is the current test for parsing through content.opf.

## Required Environment
To use expat, download and set up [MSYS2](https://www.msys2.org/).\
(I am pretty sure I used [this](https://code.visualstudio.com/docs/cpp/config-mingw) guide a while ago, although I don't use VSCode -Owen).\
Next, open the MSYS2 MINGW64 terminal, and run `pacman -S mingw-w64-x86_64-expat`

### Compiling
Open the MSYS2 MINGW64 terminal, and run `cd /c/<Rest of Directory>`\
(I have personally just been compiling it directly from a folder named EPubTest in my C drive with all the files from the firmware folder, so my command is `cd /c/EPubTest` - Owen)\
Then run `gcc Parsing.c -o Parsing -lexpat`

### Executing
Open the Command Prompt (Windows + R, type "cmd")\
Run `cd \<Directory>` (`cd \EPubTest`)\
Then run `Parsing.exe`

## Current Limitations
I have no knowledge on running programs across operating systems, so if you are using Linux, this could be impossible or very easy to run.\
I also don't have much experience with the command line, so most of the actual compiling was me interrogating ChatGPT. If you have any problems, probably ask it. - Owen
