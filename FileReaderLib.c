#define BLOCKSIZE 1024

char *readAllocLine(FILE *fp) {
    char    *line = NULL;
    size_t  maxlength = 0;
    assert(fp != NULL);
    for(;;) { // Read the line in BLOCKSIZE -blocks.
        char *crlf, *block;

        maxlength += BLOCKSIZE;
        // This exploits realloc behaviour upon hitting NULL
        if (NULL == (line = realloc(line, maxlength+1))) {
            break; // realloc error returns NULL.
        }
        block = line + maxlength - BLOCKSIZE;
        // BLOCKSIZE+1 to accommodate final zero
        if (NULL == fgets(block, BLOCKSIZE+1, fp)) {
            // TODO: rewind fp in case of error.
            if (block == line) {
                    // Error.
                    free(line); line = NULL;
            }
            break;
        }
        // This was the last block iff we find a CRLF inside.
        if (NULL != (crlf = strchr(block, '\n'))) {
            *crlf = 0x0;
            if (crlf != block) {
                if ('\r' == *(--crlf))
                    *crlf = 0x0;
            }
            break;
        } /* if */
    } /* for */
    return line;
}