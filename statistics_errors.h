#ifndef H_ERRORS_H
#define H_ERRORS_H

/*
 * These errors should be handled properly by detecting them and
 * returning the proper status code by calling exit(enum error)
 */

enum error
{
	INCORRECT_COMMAND_LINE_ARGUMENTS = 1,
        INVALID_COMMAND_INPUT, //2
	INPUT_FILE_FAILED_TO_OPEN, //3
	INPUT_FILE_FAILED_TO_CLOSE, //4
        PARSING_ERROR_INVALID_CHARACTER_ENCOUNTERED_LOG, //5
        PARSING_ERROR_TIME_DECREASING_LOG, //6
	OUTPUT_FILE_FAILED_TO_OPEN, //7
	OUTPUT_FILE_FAILED_TO_CLOSE, //8
        DID_NOT_FIND_ROBOTS_OR_ENDSPACES//9,    
};

#endif
