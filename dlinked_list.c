   /* File:     dlinked_list.c
    * Author:   Dejan Djokic
    * Section:  CS 220 - 02
    * Mentor:   Peter Pacheco
    *
    * Purpose:  Implement a sorted double-linked list with ops insert (at head),
    *           print, member, delete, free_list.
    * 
    * Input:    Single character lower case letters to indicate operators, 
    *           followed by arguments needed by operators.
    * Output:   Results of operations.
    *
    * Compile:  gcc -g -Wall -o linked_list dlinked_list.c
    * Run:      ./linked_list
    *
    * Notes:
    *    1.  Repeated values are allowed in the list
    *    2.  delete only deletes the first occurrence of a value
    *    3.  Program assumes an int will be entered when prompted
    *        for one.
    */
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>

   const int MAX_STRING_LEN = 99;

   struct list_node_s {
      char   *data;
      struct list_node_s* next_p;
      struct list_node_s* prev_p;
   };

   struct list_s {
      struct list_node_s* h_p;
      struct list_node_s* t_p;
   };

   void Delete(struct list_s* list, char *s);
   void Insert(struct list_s* list, char *s);
   void Print(struct list_s* list);
   void Free_list(struct list_s* list);
   int Member(struct list_s* list, char *s);

   char Get_command(void);
   char* Get_string(char *s);
   struct list_node_s* Allocate_node(int size);
   void Free_node(struct list_node_s* node_p);

   int main(void) {
      char input[MAX_STRING_LEN];
      char command;
      struct list_s list;
      list.h_p = NULL;
      list.t_p = NULL;
         /* start with empty list */

      command = Get_command();
      while (command != 'q' && command != 'Q') {
         switch (command) {
            case 'i':
            case 'I':
            	printf("Please input new string: \n");
               Get_string(input);
               Insert(&list, input);
               break;
            case 'p':
            case 'P':
               Print(&list);
               break;
            case 'm': 
            case 'M':
            	printf("Please input the string you are looking for\n");
               Get_string(input);
               if (Member(&list, input))
                  printf("%s is in the list\n", input);
               else
                  printf("%s is not in the list\n", input);
               break;
            case 'd':
            case 'D':
            	printf("Please input the string you wish to delete\n");
               Get_string(input);
               Delete(&list, input);
               break;
            case 'f':
            case 'F':
               Free_list(&list);
               break;
            default:
               printf("There is no %c command\n", command);
               printf("Please try again\n");
         }
         command = Get_command();
      }
      return 0;
   }  /* main */

      /*-----------------------------------------------------------------
    * Function:   Delete
    * Purpose:    Delete the first occurrence of string from list
    * Input args: list: containts pointer to head of list
    *             *s:    string
    */
   void Delete(struct list_s* list, char *s) {
      struct list_node_s* curr_p = list -> h_p;
      int result;
      /* Find node containing val and predecessor of this node */
      while(curr_p != NULL) {
   	    result = strcmp((curr_p -> data), s);
   	    if(result == 0) {
   	   		break;	   			
   	   	}
   	   	curr_p = curr_p -> next_p;
      }

      if (curr_p == NULL) {
         printf("%s is not in the list\n", s);
      }
      else if(curr_p -> prev_p == NULL && curr_p -> next_p == NULL) {
   		list -> h_p = NULL;
      		list -> t_p = NULL;
      		Free_node(curr_p);
      		return;
      	}
      else if(curr_p -> prev_p == NULL && curr_p -> next_p != NULL) {
      		list -> h_p = curr_p -> next_p;
      		(curr_p -> next_p) -> prev_p = NULL; 
      		Free_node(curr_p);
      		return;
      	}
      else if(curr_p -> prev_p != NULL && curr_p -> next_p == NULL) {
      		list -> t_p = curr_p -> prev_p;
      		(curr_p -> prev_p) -> next_p = NULL;
         	Free_node(curr_p);
         	return;
       }
      else {
         	(curr_p -> prev_p) -> next_p = curr_p -> next_p;
         	(curr_p -> next_p) -> prev_p = curr_p -> prev_p;
         	Free_node(curr_p);
         	return;
   	} 
   } /* Delete */

   /*-----------------------------------------------------------------
    * Function:   Insert
    * Purpose:    Insert string in the sorted dobule linked list
    * Input args: list: containts pointer to head of list
    *             s:  new string to be inserted
    */
   void Insert(struct list_s* list, char *s){
      int length = strlen(s), result;
      struct list_node_s* temp_p;
      struct list_node_s* curr_p = list -> h_p;

      temp_p = Allocate_node(length);
      strcpy((temp_p -> data), s);

      // if the list is empty
      if (list -> h_p == NULL) {
      		temp_p -> prev_p = NULL;
      		temp_p -> next_p = NULL;
      		list -> h_p = temp_p;
      		list -> t_p = temp_p;
      		return;
      }

      while(curr_p -> next_p!= NULL) {
   	    result = strcmp((curr_p -> data), s);
   	    if(result > 0) 
   	   		break;
   	   	if(result == 0){
   	   		printf("%s string is already in the list, unable to add!\n", s);
   	   		return;
   	   	}
   	   	curr_p = curr_p -> next_p;
      }

      // checking if it is the tail
      result = strcmp((curr_p -> data), (temp_p -> data));
      if (result == 0) {
      		printf("%s string is already in the list, unable to add!\n", s);
      		return;
    	}

      // start of the list
      if (result > 0) {
       	if(curr_p -> prev_p == NULL) {
         		temp_p -> next_p = list -> h_p;
         		(list -> h_p) -> prev_p = temp_p;
      			list -> h_p = temp_p;
      			temp_p -> prev_p = NULL;
      			return;
      		}
       }  		
       // end of the list
      	if (result < 0) {
      		if(curr_p -> next_p == NULL) {
      			temp_p -> prev_p = list -> t_p;
     			(list -> t_p) -> next_p = temp_p;
    			list -> t_p = temp_p;
      			temp_p -> next_p = NULL;
   		   	return;
   		}
   	}
   	// in between
      	else {
      		temp_p -> next_p = curr_p;
      		temp_p -> prev_p = curr_p -> prev_p;
      		(curr_p -> prev_p) -> next_p = temp_p;
      		curr_p -> prev_p = temp_p;
   	}
   }  /* Insert */

      /*-----------------------------------------------------------------
    * Function:   Print
    * Purpose:    Print list on a single line of stdout
    * Input arg:  list: containts pointer to head of list
    */
   void Print(struct list_s* list) {
      struct list_node_s* curr_p = list -> h_p;

      printf("list = ");
      while (curr_p != NULL) {
         printf("%s ", curr_p -> data);
         curr_p = curr_p->next_p;
      }
      printf("\n");
   }  /* Print */
      /*-----------------------------------------------------------------
    * Function:    Member
    * Purpose:     search list for string
    * Input args:  list: containts pointer to head of list
    *              s:       value to search for
    * Return val:  1 if val is in list, 0 otherwise
    */
   int Member(struct list_s* list, char *s) {
      struct list_node_s* curr_p = list -> h_p;
      int result;

      while (curr_p != NULL) {
        	result = strcmp((curr_p -> data), s);
   	    if(result == 0) 	   			
           	return 1;
           else
            	curr_p = curr_p -> next_p;
       	}
      return 0;
   }  /* Member */

      /*-----------------------------------------------------------------
    * Function:   Free_node
    * Purpose:    Frees the storage for the string, and then frees the
    *             storage for the node
    * Input arg:  node_p
    */
   void Free_node(struct list_node_s* node_p) {
   	free(node_p->data);
   	free(node_p);
   } /* Free_node */

   	/*-----------------------------------------------------------------
    * Function:    Free_list
    * Purpose:     free each node in the list
    * Input arg:   list: containts pointer to head of list
    * Note:        head_p is set to NULL on completion, indicating
    *              list is empty.
    */
   void Free_list(struct list_s* list) {
      struct list_node_s* curr_p;
      struct list_node_s* temp_p;

      curr_p = list -> h_p;
      while (curr_p != NULL) {
         temp_p = curr_p;
         curr_p = curr_p -> next_p;
         Free_node(temp_p);     
      }

      list -> h_p = NULL;
   }  /* Free_list */	
   /*-----------------------------------------------------------------
    * Function:   Get_string
    * Purpose:    Get a string from stdin
    * Return value:  string s
    * Note:       Behavior unpredictable if nothing entered
    */
   char* Get_string(char *s) {
   	scanf("%99s", s);
   	return s;
   } /* Get_string */	

   	/*-----------------------------------------------------------------
    * Function:    Allocate_node
    * Purpose:     Allocates storage for a node
    * Input arg:   size: size for a string of the desired length
    * Return val:  temp_p - newly created node
    */
   struct list_node_s* Allocate_node(int size){
   	struct list_node_s* temp_p;
   	temp_p = malloc(sizeof(struct list_node_s));
   	temp_p->data = malloc((size+1)*sizeof(char));
   	return temp_p;
   }/* Allocate_node */

   /*-----------------------------------------------------------------
    * Function:      Get_command
    * Purpose:       Get a single character command from stdin
    * Return value:  the first non-whitespace character from stdin
    */

   char Get_command(void) {
      char c;

      printf("Please enter a command (i, p, m, d, f, q):  ");
      /* Put the space before the %c so scanf will skip white space */
      scanf(" %c", &c);
      return c;
   }  /* Get_command */
