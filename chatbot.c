/*
 * ICT1002 (C Language) Group Project, AY19 Trimester 1.
 *
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */

#include <stdio.h>
#include <string.h>
#include "chat1002.h"

/*
 * Get the name of the chatbot.
 *
 * Returns: the name of the chatbot as a null-terminated string
 */
const char *chatbot_botname()
{

	return "Javier Lim";
}

/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char *chatbot_username()
{

	return "User";
}

/*
 * Get a response to user input.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop (i.e. it detected the EXIT intent)
 */
int chatbot_main(int inc, char *inv[], char *response, int n)
{

	/* check for empty input */
	if (inc < 1)
	{
		snprintf(response, n, "");
		return 0;
	}

	/* look for an intent and invoke the corresponding do_* function */
	if (chatbot_is_exit(inv[0]))
		return chatbot_do_exit(inc, inv, response, n);
	else if (chatbot_is_smalltalk(inv[0]))
		return chatbot_do_smalltalk(inc, inv, response, n);
	else if (chatbot_is_load(inv[0]))
		return chatbot_do_load(inc, inv, response, n);
	else if (chatbot_is_question(inv[0]))
		return chatbot_do_question(inc, inv, response, n);
	else if (chatbot_is_reset(inv[0]))
		return chatbot_do_reset(inc, inv, response, n);
	else if (chatbot_is_save(inv[0]))
		return chatbot_do_save(inc, inv, response, n);
	else if (chatbot_is_help(inv[0]))
		return chatbot_do_help(inc, inv, response, n);
	else if (chatbot_is_help2(inv[0]))
		return chatbot_do_help2(inc, inv, response, n);
	else
	{
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}
}

/*
 * Determine whether an intent is HELP.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "help" or "/help"
 *  0, otherwise
 */
int chatbot_is_help(const char* intent)
{
	return compare_token(intent, "help") == 0 || compare_token(intent, "/help") == 0;
}

int chatbot_is_help2(const char* intent)
{
	return compare_token(intent, "help2") == 0 || compare_token(intent, "/help2") == 0;
}

/*
 * Perform the HELP intent.
 *
 * Returns:
 *   0 
 */
int chatbot_do_help2(int inc, char* inv[], char* response, int n)
{
	snprintf(response, n, "\nsave filename.ini      Save the current data in the chatbot into an .ini file.\nreset                  Reset the chatbot to its original state.\nexit                   Exit the program.");
	return 0;
}

int chatbot_do_help(int inc, char* inv[], char* response, int n)
{
	snprintf(response, n, "\nhelp                   Show this page. help2 for next page.\nload filename          load a .ini file into the chatbot.\nredefine question      Change the data stored in the chatbot for question provided.\nweather(?)             Get the weather.");
	return 0;
}

/*
 * Determine whether an intent is EXIT.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
int chatbot_is_exit(const char *intent)
{

	return compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0;
}

/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char *inv[], char *response, int n)
{

	snprintf(response, n, "Goodbye!");

	return 1;
}

/*
 * Determine whether an intent is LOAD.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "load"
 *  0, otherwise
 */
int chatbot_is_load(const char *intent)
{
	return compare_token(intent, "load") == 0;
}

/*
 * Load a chatbot's knowledge base from a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_load(int inc, char *inv[], char *response, int n)
{

	int no_of_responses_read = 0;
	char filename[MAX_ENTITY];
	char removed[MAX_ENTITY];
	char *ignorelist[] = {"from"};

	FILE *file = fopen(inv[1], "r");

	strcpy(filename, inv[1]);

	if (file == NULL)
	{
		snprintf(response, n, "%s does not exist", filename);
	}
	else
	{
		no_of_responses_read = knowledge_read(file);
		fclose(file);
		snprintf(response, n, "Read %d responses from %s.", no_of_responses_read, filename);
	}

	return 0;
}

/*
 * Determine whether an intent is a question.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_question(const char *intent)
{

	/* to be implemented */

	//return 0;
	return compare_token(intent, "what") == 0 || compare_token(intent, "where") == 0 || compare_token(intent, "who") == 0;
}

/*
 * Answer a question.
 *
 * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * The remainder of the words form the entity.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_question(int inc, char *inv[], char *response, int n)
{

	char *intent = inv[0];
	char entity[MAX_ENTITY] = "\0";
	char removed[MAX_ENTITY] = "\0";
	char *ignore[] = {"is", "are"};

	if (2 < inc)
	{
		for (int i = 2; i < inc; i++)
		{
			if (i == 2)
			{
				strcat(entity, inv[i]);
			}
			else
			{
				strcat(entity, " ");
				strcat(entity, inv[i]);
			}
		}
	}
	else
	{
		strncat(entity, inv[2], MAX_RESPONSE);
	}

	if(knowledge_get(intent, entity, response, n) == KB_NOTFOUND){
		printf("not found\n");
		char answer[MAX_RESPONSE];
		strcat(removed, inv[1]);
		prompt_user(answer, n,"I don't know. %s %s %s?",intent,removed,entity);

		printf("%s\n", intent);
		printf("%s\n", entity);
		printf("%s\n", answer);

        knowledge_put(intent,entity, answer);
		snprintf(response, n, "Thank you."); 
	}
	//knowledge_get(intent, entity, response, n);

	/* to be implemented */

	return 0;
}

/*
 * Determine whether an intent is RESET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */
int chatbot_is_reset(const char *intent)
{

	/* to be implemented */

	return 0;
}

/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after beign reset)
 */
int chatbot_do_reset(int inc, char *inv[], char *response, int n)
{

	/* to be implemented */

	return 0;
}

/*
 * Determine whether an intent is SAVE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_save(const char *intent)
{
	
	/* to be implemented */

	return compare_token(intent, "save") == 0;
}

/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char *inv[], char *response, int n)
{

	/* to be implemented */
	FILE *f;
	char filename[MAX_ENTITY] = "\0";
	strcpy(filename, inv[1]);
	f = fopen(filename, "w+");
	knowledge_write(f);
	fclose(f);
	snprintf(response, n, "File has been saved");
	return 0;
	
	return 0;
}

/*
 * Determine which an intent is smalltalk.
 *
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is the first word of one of the smalltalk phrases
 *  0, otherwise
 */
int chatbot_is_smalltalk(const char *intent)
{
	
	return compare_token(intent, "hi") == 0 ||
	compare_token(intent, "morning") == 0 ||
	compare_token(intent, "motivation") == 0 ||
	compare_token(intent, "afternoon") == 0 ||
	compare_token(intent, "nights") == 0;
	
	/* to be implemented */

	return 0;
}


replybank knowledgeBank[] = {
	//responses for "hi"//
	{"hi",
	{"hello there!", "it's nice to see you!","Ann yeongg!"}
	},

	//responses for "good morning"//
	{"morning",
	{"A very good morning to you!","Ohayōgozaimasu senpai!","Zhao an ni hao!"}
	},

	//responses for "I need some motivation"//
	{"motivation",
	{"Yes sir!, If everything seems under control, you're not going fast enough ",
		"Yes sir!, Courage is not the absence of fear, but the triumph over it!",
		"Yes sir!,You will survive because the fire inside you will burn brighter than the fire around you!"}
	},

	//responses for "good afternoon"//
	{"afternoon",
	{"A very good afternoon to you too!","Time for some lunch!","It's time to get working!"}
	},

	//responses for "good night"//
	{"nights",
	{"Hope you have a good rest!","Good night to you!","Sweet dreams!"}
	},
};
/*
 * Respond to smalltalk.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop chatting (e.g. the smalltalk was "goodbye" etc.)
 */
int chatbot_do_smalltalk(int inc, char *inv[], char *response, int n)
{
	char* smalltalkoutput;
	for (int i = 0; i < sizeof(knowledgeBank) / sizeof(knowledgeBank[0]); ++i) {

		if (strcmp(knowledgeBank[i].intent, inv[0]) == 0) {
			int r = rand() % 3; //random number from 0-2 for a randomized response
			smalltalkoutput = knowledgeBank[i].responses[r];
			snprintf(response, n, smalltalkoutput);
		}

	}
	return 0;
	/* to be implemented */

	
}
