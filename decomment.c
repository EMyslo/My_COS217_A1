/* decomment.c*/

#include <stdio.h>
#include <stdlib.h> 

/* Defines constants representing the states in the DFA*/
enum Statetype {NORMAL, MAYBE_COMMENT, IN_COMMENT, MAYBE_END_COMMENT, STRING_LIT, STRING_ESCAPE, CHAR_LIT, CHAR_ESCAPE};

/* Implement the NORMAL state of the DFA, where NORMAL refers to any text that is not part of a comment. c is the current DFA character. Depending on the value of c, either writes c to to stdout or suppresses c while transitioning to next state. Return the next state specified by the DFA.*/
enum Statetype
handleNormalState(int c)
{
   enum Statetype state;
   if (c == '/') {
      state = MAYBE_COMMENT;
   }
   else if (c == '"') {
      putchar(c);
      state = STRING_LIT;
   }
   else if (c == '\'') {
      putchar(c);
      state = CHAR_LIT;
   }
   else {
      putchar(c);
      state = NORMAL;
   }
   return state;
}

/* Implement the MAYBE_COMMENT state of the DFA. c is the current DFA character. Depending on the value of c, either writes c (possibly preceded by a previously-suppressed forward slash) to stdout or writes a whitespace to stdout. Return the next state specified by the DFA.*/
enum Statetype
handleMaybeCommentState(int c)
{
   enum Statetype state;
   if (c =='*') {
      putchar(' ');
      state = IN_COMMENT;  
   }
   else if (c == '"') {
      putchar('/');
      putchar(c);
      state = STRING_LIT;
   }
   else if (c == '\'') {
      putchar('/');
      putchar(c);
      state = CHAR_LIT;
   }
   else if (c == '/') {
      putchar(c);
      state = MAYBE_COMMENT;
   }
   else {
      putchar('/');
      putchar(c);
      state = NORMAL;
   }
   return state;
}
   
/* Implement the IN_COMMENT state of the DFA. c is the current DFA character. Does not write to stdout while in a comment. Return the next state specified by the DFA*/
enum Statetype
handleInCommentState(int c)
{
   enum Statetype state;
   if (c == '*') {
      state = MAYBE_END_COMMENT;
   }
   else {
      if (c == '\n') {
         putchar(c);
      }
      state = IN_COMMENT;
   }
   return state;
}

/* Implement the MAYBE_END_COMMENT state of the DFA. c is the current DFA character. Does not write to stdout while in a comment. Return the next state specified by the DFA */
enum Statetype
handleMaybeEndCommentState(int c)
{
   enum Statetype state;
   if (c == '/') {
      state = NORMAL;
   }
   else if (c == '*') {
      state = MAYBE_END_COMMENT;
   }
   else {
      if (c == '\n') {
         putchar(c);
      }
      state = IN_COMMENT;
   }
   return state;
}

/* Implement STRING_LIT state of the DFA. c is the current DFA character. Write c to stdout while in a String literal. Return the next state specified by the DFA. */
enum Statetype
handleStringLitState(int c)
{
   enum Statetype state;
   if (c == '\\') { /*Is this the correct way to represent a single backslash?*/
      putchar(c);
      state = STRING_ESCAPE;
   }
   else if (c == '"') {
      putchar(c);
      state = NORMAL;
   }
   else {
      putchar(c);
      state = STRING_LIT;
   }
   return state;
}

/* Implement STRING_ESCAPE state of the DFA. c is the current DFA character. Write c to stdout. Return next state, which is STRING_LIT regardless of the current DFA character.*/
enum Statetype
handleStringEscapeState(int c)
{
   enum Statetype state;
   putchar(c);
   state = STRING_LIT;
   return state;
}

/* Implement CHAR_LIT state of the DFA. c is the current DFA character. Write c to stdout while in a character literal. Returns the next state specified by the DFA.*/
enum Statetype
handleCharLitState(int c)
{
   enum Statetype state;
   if (c == '\\') {
      putchar(c);
      state = CHAR_ESCAPE;
   }
   else if (c == '\'') {
      putchar(c);
      state = NORMAL;
   }
   else {
      putchar(c);
      state = CHAR_LIT;
   }
   return state;
}

/* Implement CHAR_ESCAPE state of the DFA. c is the current DFA character. Write c to stdout. Return next state, which is CHAR_LIT regardless of the current DFA character.*/
enum Statetype
handleCharEscapeState(int c)
{
   enum Statetype state;
   putchar(c);
   state = CHAR_LIT;
   return state;
}

/*Read text from stdin one charater at a time. c is the current character in the DFA. Maintains absline, the current line of code, including comments. Maintains errline, the line on which the most recently opened comment began. Writes each character that is not part of a comment to stdout. Replaces each comment with a single whitespace and writes to stdout. Return EXIT_SUCCESS if program detects no unterminated comments. Return EXIT_FAILURE if detects an unterminated comment, and specify errline as the line where unterminated comment began.*/
int main(void)
{
   int c;
   int absline = 1;
   int errline = 0;
   enum Statetype state = NORMAL;
   while ((c = getchar()) != EOF) {
      switch (state) {
         case NORMAL:
            state = handleNormalState(c);
            break;
         case MAYBE_COMMENT:
            state = handleMaybeCommentState(c);
            if (state == IN_COMMENT) {
               errline = absline;
            }
            break;
         case IN_COMMENT:
            state = handleInCommentState(c);
            break;
         case MAYBE_END_COMMENT:
            state = handleMaybeEndCommentState(c);
            break;
         case STRING_LIT:
            state = handleStringLitState(c);
            break;
         case STRING_ESCAPE:
            state = handleStringEscapeState(c);
            break;
         case CHAR_LIT:
            state = handleCharLitState(c);
            break;
         case CHAR_ESCAPE:
            state = handleCharEscapeState(c);
            break;
      }
      if (c == '\n') {
         absline++;
      }
      
   }

   if (state == MAYBE_COMMENT) {
         putchar('/');
      }
   
   if (state == IN_COMMENT || state == MAYBE_END_COMMENT) {
      /* Is the following fprintf correct way of incorporting the variable errline?*/
      fprintf(stderr, "Error: line %d: unterminated comment\n", errline);
      return EXIT_FAILURE;
   }

   return 0;
}
