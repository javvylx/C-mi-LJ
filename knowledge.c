/*
 * ICT1002 (C Language) Group Project, AY19 Trimester 1.
 *
 * This file implements the chatbot's knowledge base.
 *
 * knowledge_get() retrieves the response to a question.
 * knowledge_put() inserts a new response to a question.
 * knowledge_read() reads the knowledge base from a file.
 * knowledge_reset() erases all of the knowledge.
 * kowledge_write() saves the knowledge base in a file.
 *
 * You may add helper functions as necessary.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "chat1002.h"
#include <Windows.h>

struct linked_list
{
	char entity[MAX_ENTITY];
	char response[MAX_RESPONSE];
	struct linked_list *next;
};

typedef struct linked_list wholist;
typedef struct linked_list whatlist;
typedef struct linked_list wherelist;

wholist *whohead = NULL, *whotail = NULL;
whatlist *whathead = NULL, *whattail = NULL;
wherelist *wherehead = NULL, *wheretail = NULL;

/*
 * Get the response to a question.
 *
 * Input:
 *   intent   - the question word
 *   entity   - the entity
 *   response - a buffer to receive the response
 *   n        - the maximum switchcaseber of characters to write to the response buffer
 *
 * Returns:
 *   KB_OK, if a response was found for the intent and entity (the response is copied to the response buffer)
 *   KB_NOTFOUND, if no response could be found
 *   KB_INVALID, if 'intent' is not a recognised question word
 */
int knowledge_get(const char *intent, const char *entity, char *response, int n)
{
	int switchcase = 0;
	if (compare_token(intent, "who") == 0)
	{
		switchcase = 0;
	}
	else if (compare_token(intent, "what") == 0)
	{
		switchcase = 1;
	}
	else if (compare_token(intent, "where") == 0)
	{
		switchcase = 2;
	}
	else
	{
		return KB_INVALID;
	}

	switch (switchcase)
	{
	case 0:
		{wholist *tempnode = whohead;
		int flag = 0;
		while (tempnode != NULL)
		{
			if (compare_token(tempnode->entity,entity)==0)
			{
				snprintf(response, n, "%s", tempnode->response);
				flag = 1;
				return KB_OK;
			}
			else
				tempnode = tempnode->next;
		}
		if (flag == 0)
			return KB_NOTFOUND;
		break;
		}

	case 1:
		{whatlist *tempnode = whathead;
		int flag = 0;
		while (tempnode != NULL)
		{
			if (compare_token(tempnode->entity,entity)==0)
			{
				snprintf(response, n, "%s", tempnode->response);
				flag = 1;
				return KB_OK;
			}
			else
				tempnode = tempnode->next;
		}
		if (flag == 0)
			return KB_NOTFOUND;
		break;
		}

	case 2:
		{wherelist *tempnode = wherehead;
		int flag = 0;
		while (tempnode != NULL)
		{
			if (compare_token(tempnode->entity,entity)==0)
			{
				snprintf(response, n, "%s", tempnode->response);
				flag = 1;
				return KB_OK;
			}
			else
				tempnode = tempnode->next;
		}
		if (flag == 0)
			return KB_NOTFOUND;
		break;
		}

	default:
		return KB_INVALID;
	}
}

/*
 * Insert a new response to a question. If a response already exists for the
 * given intent and entity, it will be overwritten. Otherwise, it will be added
 * to the knowledge base.
 *
 * Input:
 *   intent    - the question word
 *   entity    - the entity
 *   response  - the response for this question and entity
 *
 * Returns:
 *   KB_FOUND, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
int knowledge_put(const char *intent, const char *entity, const char *response)
{

	if (compare_token(intent, "who") == 0)
	{
		wholist *tempnode;
		tempnode = (wholist *)malloc(sizeof(wholist));

		strcpy(tempnode->entity, entity);
		strcpy(tempnode->response, response);
		tempnode->next = NULL;

		//For the 1st element
		if (whohead == NULL)
		{
			whohead = tempnode;
			whotail = tempnode;
		}
		else
		{
			whotail->next = tempnode;
			whotail = tempnode;
		}
	}
	else if (compare_token(intent, "what") == 0)
	{
		whatlist *tempnode;
		tempnode = (whatlist *)malloc(sizeof(whatlist));

		strcpy(tempnode->entity, entity);
		strcpy(tempnode->response, response);
		tempnode->next = NULL;

		//For the 1st element
		if (whathead == NULL)
		{
			whathead = tempnode;
			whattail = tempnode;
		}
		else
		{
			whattail->next = tempnode;
			whattail = tempnode;
		}
	}
	else if (compare_token(intent, "where") == 0)
	{
		wherelist *tempnode;
		tempnode = (wherelist *)malloc(sizeof(wherelist));

		strcpy(tempnode->entity, entity);
		strcpy(tempnode->response, response);
		tempnode->next = NULL;

		//For the 1st element
		if (wherehead == NULL)
		{
			wherehead = tempnode;
			wheretail = tempnode;
		}
		else
		{
			wheretail->next = tempnode;
			wheretail = tempnode;
		}
	}

	return KB_INVALID;
}

/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the switchcaseber of entity/response pairs successful read from the file
 */
int knowledge_read(FILE *f)
{

	char line[128];
	int lines_read = 0;
	int switchcase = 0;

	//entity and response
	char *entity;
	char *response;

	while (fgets(line, MAX_ENTITY + 1 + MAX_RESPONSE + 1, f))
	{
		line[strcspn(line, "\n")] = 0;

		for (int i = 0; i < strlen(line); i++)
		{
			if (isalpha(line[i]) != 0 && !strstr(line, "="))
			{
				char *lineptr = strtok(line, "[");
				char *lineintent = strtok(lineptr, "]");

				if (compare_token(lineintent, "who") == 0)
				{
					switchcase = 0;
				}
				if (compare_token(lineintent, "what") == 0)
				{
					switchcase = 1;
				}
				if (compare_token(lineintent, "where") == 0)
				{
					switchcase = 2;
				}
			}
			else if (isalpha(line[i]) == 0 && strstr(line, "="))
			{
				char *entity = strtok(line, "=");
				char *response = strtok(NULL, "=");
				switch (switchcase)
				{
				case 0:
					lines_read++;
					knowledge_put("who", entity, response);
					break;

				case 1:
					lines_read++;
					knowledge_put("what", entity, response);
					break;

				case 2:
					lines_read++;
					knowledge_put("where", entity, response);
					break;

				default:
					break;
				}
			}
		}
	}

	return lines_read;
}

/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset()
{

	/* to be implemented */
}

/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE *f)
{
	// What intent
	fputs("[what]\n", f);
	while (whathead != NULL)
	{
		fprintf(f, "%s = %s\n", whathead->entity, whathead->response);
		whathead = whathead->next;
	}

	// Where intent
	fputs("\n[where]\n", f);
	while (wherehead != NULL)
	{
		fprintf(f, "%s = %s\n", wherehead->entity, wherehead->response);
		wherehead = wherehead->next;
	}

	// Who intent
	fputs("\n[who]\n", f);
	while (whohead != NULL)
	{
		fprintf(f, "%s = %s\n", whohead->entity, whohead->response);
		whohead = whohead->next;
	}
	/* to be implemented */
}