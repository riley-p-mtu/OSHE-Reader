#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "miniz.h"
#include <expat.h>
#define MINIZ_IMPLEMENTATION
#include "miniz.c"

// Helper Structs
typedef struct {
    char id[128];
    char href[256];
} manifest_item_t;

typedef struct {
    manifest_item_t manifest[200];
    int manifest_count;
    char spine_ids[200][128];
    int spine_count;
} opf_data_t;

typedef struct {
    char opf_path[256];
} container_data_t;

// Extract file 
void *read_epub_file(mz_zip_archive *zip, const char *path, size_t *out_size) {
    void *p = mz_zip_reader_extract_file_to_heap(zip, path, out_size, 0);
    if (!p) {
        printf("Failed to extract %s\n", path);
        return NULL;
    }
    return p;
}

// container.xml Parser
void start_container(void *user_data, const char *name, const char **atts) {
    container_data_t *data = (container_data_t *)user_data;
    if (strcmp(name, "rootfile") == 0) {
        for (int i = 0; atts[i]; i += 2)
            if (strcmp(atts[i], "full-path") == 0)
                strcpy(data->opf_path, atts[i + 1]);
    }
}

// Looks for path to container
char *find_opf_path(const char *xml, size_t len) {
    XML_Parser parser = XML_ParserCreate(NULL);
    container_data_t data = {0};

    XML_SetUserData(parser, &data);
    XML_SetElementHandler(parser, start_container, NULL);

    if (XML_Parse(parser, xml, len, 1) == XML_STATUS_ERROR)
        fprintf(stderr, "XML Parse error (container.xml): %s\n",
                XML_ErrorString(XML_GetErrorCode(parser)));

    XML_ParserFree(parser);
    return strdup(data.opf_path);
}

// content.opf Parser
void start_opf(void *user_data, const char *name, const char **atts) {
    opf_data_t *opf = (opf_data_t *)user_data;

    if (strcmp(name, "item") == 0) {
        manifest_item_t item = {0};
        for (int i = 0; atts[i]; i += 2) {
            if (strcmp(atts[i], "id") == 0)
                strcpy(item.id, atts[i + 1]);
            else if (strcmp(atts[i], "href") == 0)
                strcpy(item.href, atts[i + 1]);
        }
        opf->manifest[opf->manifest_count++] = item;
    } 
    else if (strcmp(name, "itemref") == 0) {
        for (int i = 0; atts[i]; i += 2)
            if (strcmp(atts[i], "idref") == 0)
                strcpy(opf->spine_ids[opf->spine_count++], atts[i + 1]);
    }
}

// Parse content.opf
void parse_opf(const char *xml, size_t len, opf_data_t *opf) {
    XML_Parser parser = XML_ParserCreate(NULL);
    XML_SetUserData(parser, opf);
    XML_SetElementHandler(parser, start_opf, NULL);

    if (XML_Parse(parser, xml, len, 1) == XML_STATUS_ERROR)
        fprintf(stderr, "XML Parse error (content.opf): %s\n",
                XML_ErrorString(XML_GetErrorCode(parser)));

    XML_ParserFree(parser);
}

// XHTML Parser for first chapter detection
static int in_chapter = 0;
static int chapter_found = 0;
static int stop_after_chapter = 0;

static char current_tag[32] = "";
static char last_heading[256] = "";

void trim_and_lower(char *s) {
    // Trim spaces
    char *p = s;
    while (isspace((unsigned char)*p)) p++;
    memmove(s, p, strlen(p)+1);
    for (int i = strlen(s) - 1; i >= 0 && isspace((unsigned char)s[i]); i--)
        s[i] = '\0';
    // Lowercase
    for (int i = 0; s[i]; i++)
        s[i] = (char)tolower((unsigned char)s[i]);
}

// Looks for something resembling a chapter heading
int chapter_heading(const char *text) {
    char tmp[256];
    strncpy(tmp, text, sizeof(tmp));
    tmp[sizeof(tmp)-1] = '\0';
    trim_and_lower(tmp);
    if (strstr(tmp, "chapter")) return 1;
    return 0;
}

void start_xhtml(void *user_data, const char *name, const char **atts) {
    strcpy(current_tag, name);
    if (strcmp(name, "h1") == 0 || strcmp(name, "h2") == 0)
        printf("\n\n");
}

void end_xhtml(void *user_data, const char *name) {
    current_tag[0] = '\0';
}

void char_data_xhtml(void *user_data, const char *s, int len) {
    if (stop_after_chapter) return;

    char text[512];
    snprintf(text, sizeof(text), "%.*s", len, s);

    if (strcmp(current_tag, "h1") == 0 || strcmp(current_tag, "h2") == 0) {
        // Look for chapter heading
        if (!chapter_found && chapter_heading(text)) {
            chapter_found = 1;
            in_chapter = 1;
            printf("=== %s ===\n", text);
            return;
        }
        else if (chapter_found && chapter_heading(text)) {
            // Next chapter reached
            stop_after_chapter = 1;
            return;
        }
    }

    if (in_chapter)
        printf("%.*s", len, s);
}

void parse_xhtml(const char *xml, size_t len) {
    XML_Parser parser = XML_ParserCreate(NULL);
    XML_SetElementHandler(parser, start_xhtml, end_xhtml);
    XML_SetCharacterDataHandler(parser, char_data_xhtml);

    XML_Parse(parser, xml, len, 1);
    XML_ParserFree(parser);
}

// Main
int main() {
    static const char *file_name = "MobyDick.epub";
    mz_zip_archive zip = {0};

    if (!mz_zip_reader_init_file(&zip, file_name, 0)) {
        printf("Failed to open %s\n", file_name);
        return 1;
    }

    // Look for container file
    size_t container_size;
    void *container_xml = read_epub_file(&zip, "META-INF/container.xml", &container_size);
    if (!container_xml) {
		return 1;
	}
	
    char *opf_path = find_opf_path(container_xml, container_size);
    mz_free(container_xml);
    printf("Found OPF path: %s\n", opf_path);

    // Extract content.opf
    size_t opf_size;
    void *opf_xml = read_epub_file(&zip, opf_path, &opf_size);
    if (!opf_xml) {
		return 1;
	}

    opf_data_t opf = {0};
    parse_opf(opf_xml, opf_size, &opf);
    mz_free(opf_xml);

    printf("Spine count: %d\n", opf.spine_count);

    // Read spine files until chapter is found and finished
    for (int i = 0; i < opf.spine_count && !stop_after_chapter; i++) {
        char *id = opf.spine_ids[i];
        for (int j = 0; j < opf.manifest_count && !stop_after_chapter; j++) {
            if (strcmp(opf.manifest[j].id, id) == 0) {
                char path[512];
                const char *last_slash = strrchr(opf_path, '/');
                if (last_slash)
                    snprintf(path, sizeof(path), "%.*s/%s",
                             (int)(last_slash - opf_path), opf_path, opf.manifest[j].href);
                else
                    snprintf(path, sizeof(path), "%s", opf.manifest[j].href);

                size_t html_size;
                void *html_data = read_epub_file(&zip, path, &html_size);
                if (html_data) {
                    parse_xhtml(html_data, html_size);
                    mz_free(html_data);
                }
            }
        }
    }

    free(opf_path);
    mz_zip_reader_end(&zip);
    printf("\n\n[Stopped after first chapter.]\n");
    return 0;
}
