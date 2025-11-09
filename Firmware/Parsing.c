#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "miniz.h"
#include <expat.h>
#define MINIZ_IMPLEMENTATION
#include "miniz.c"

// Trying to use comments to give context, but definitely read the
// README.md in the firmware folder to access the documentation

void *read_epub_file(mz_zip_archive *zip, const char *path, size_t *out_size) {
    void *p = mz_zip_reader_extract_file_to_heap(zip, path, out_size, 0);
    if (!p) {
		printf("Failed to extract %s\n", targetFile);
        mz_zip_reader_end(&zipArchive);
        return NULL;
	}
	return p;
}

// Called when Expat XML parser sees a start tag
void startElement(void *userData, const char *name, const char **atts) {
	// Just checking for title
	if (strcmp(name, "dc:title") == 0) {
		printf("Found title start tag\n");
		// Checking for contents in the spine
	} else if (strcmp(name, "itemref") == 0) {
		for (int i = 0; atts[i]; i += 2) {
			if (strcmp(atts[i], "idref") == 0) {
				printf("Chapter reference: %s\n", atts[i + 1]);
			}
		}
	}
}

// Called when there is text between tags
void charData(void *userData, const char *s, int len) {
	printf("Text: %.*s\n", len, s);
}

// Currently all this program does is print text from between the <tags> 
// in the content.opf file. Once this is reliable, we can go about finding actual
// pages and whatnot
int main() {
    static const char *fileName = "MobyDick.epub";		// Epub name
    mz_zip_archive zipArchive;							// Represents Epub
	void *p;											// Pointer used for decompression later	
	size_t uncompSize;									// Number of bytes extracted from content.opf
	
	memset(&zipArchive, 0, sizeof(zipArchive));						// Clear memory for the zip

	// Fail if it can't find Epub
    if (!mz_zip_reader_init_file(&zipArchive, fileName, 0)) {
        printf("Failed to open %s\n", fileName);
        return 1;
    }

    const char *targetFile = "OEBPS/content.opf";		// File path to file containing title and reading order

	// Try to extract content to heap memory
	p = mz_zip_reader_extract_file_to_heap(&zipArchive, targetFile, &uncompSize, 0);

	size_t opf_size;
	void *opf_data = read_epub_file(&zip, targetFile, *opf_size);
	
	XML_Parser parser = XML_ParserCreate(NULL);			// Create new parser
	XML_SetElementHandler(parser, startElement, NULL);	// Handler for calling a function when start tag is detected
	XML_SetCharacterDataHandler(parser, charData);		// Handler for when text is detected
	
	// Pass buffer data to parser, print error code if it fails
	if (XML_Parse(parser, (const XML_Char *)p, (int)uncompSize, 1) == XML_STATUS_ERROR) {
		fprintf(stderr, "XML Parse error: %s\n", XML_ErrorString(XML_GetErrorCode(parser)));
	}
	
	// Clear memory and close
	XML_ParserFree(parser);
	mz_free(p);
	mz_zip_reader_end(&zipArchive);
}