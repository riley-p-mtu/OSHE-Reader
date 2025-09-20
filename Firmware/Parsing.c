#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "miniz.h"

int main() {
    const char *fileName = "MobyDick.epub";	// Epub name
    mz_zip_archive zip;	// "zip" Represents Epub
    memset(&zip, 0, sizeof(zip));	// Clear memory for the zip

	// Fail if it can't find Epub
    if (!mz_zip_reader_init_file(&zip, fileName, 0)) {
        printf("Failed to open %s\n", fileName);
        return 1;
    }

    const char *targetFile = "OEBPS/content.opf";	// File path to file containing title and reading order


}