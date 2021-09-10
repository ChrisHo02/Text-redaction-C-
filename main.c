#include <stdio.h>
#include <stdlib.h>

//structure to store the text file read. Taken from my Prog II CW2 Q5
typedef struct file{
  //string to hold the text 
  char* text;
  //integer to hold the length 
  int characterLength;
}file;

//enum for the boolean data type.
typedef enum {false, true}boolean;

//integers to be used throughout the code. numberOfRedactedWords-number of words to be redacted. textWordCount-count of characters in text file to be redacted from.
int numberOfRedactedWords;
int textWordCount;


//Function used to read the .txt file and store it in a file struct.
file getText(char* filename){
  FILE *textFilePointer;
  long characterCount;
  char *buffer;
  //opens the text file for reading
  textFilePointer = fopen (filename , "r" );
  //if the pointer is null then there's an error as the file cannot be found
  if(textFilePointer == NULL ){
    printf("Error has occured");
    exit(0);
  }
  //sets file position of stream 
  fseek(textFilePointer,0L,SEEK_END);
  //defines characterCount as the current position of the stream. Also used to keep track of no. of characters  in the file
  characterCount = ftell(textFilePointer);
  //sets textFilePointer to beginning of stream
  rewind(textFilePointer);
  //defines buffer using calloc to read the text file.
  buffer = calloc(1,characterCount+1);
  //checks to ensure memory is allocated.
  if(buffer == NULL ){
    fclose(textFilePointer);
    printf("Memory not allocated");
    exit(0);
  }
  //if the output of fread is not equal to the no. of elements i.e. 1, then error has occured
  if(fread(buffer,characterCount,1,textFilePointer)!=1){
    fclose(textFilePointer); 
    free(buffer);
    fputs("An error has occurred or the end of the file was reached",stderr);
    exit(0);
  }
  //closes textfile for reading
  fclose(textFilePointer);
  //sets an end-pointer at the end of the buffer char array.
  buffer[characterCount] = '\0';
  //creates a file struct for the .txt file given.
  file textFile = {buffer, (int)characterCount};
  //returns the read text-file.
  return textFile;
}

//gets length of a string. Param 'word' is string given. 
int getWordLength(char* word){
  //defines i - used to iterate through array and counts length of list
  int i =0;
  //while loop continues looping through string given until end-pointer found 
  while(word[i]!='\0'){
    //i incremented by one. Acts as a counter.
    i++;
  }
  //returns length of string.
  return i;
}

//Function to convert a given word into lowercase.
char* toLower(char* word){
  //length - gets and stores length of words using getWordLength
  int length = getWordLength(word);
  //loops through given word
  for(int i=0;i<length;i++){
    //if the character is uppercase, convert it to lowercase.
    if(word[i]>64 && word[i]<91){
      word[i] = word[i]+32;
    }
  }
  //returns the word in lowercase.
  return word;
}

/**Function used to create a copy of a string 
* @param word is the string to be copied
* @param length is the length of the string to be copied
* Function used when changes shouldn't be permanent.
*/
char* copyString(char* word, int length){
  //creates char* copy to store the value of the copy.
  char* copy = (char*)malloc(length*sizeof(char));
  //checks to ensure memory has been allocated.
  if(copy == NULL){
    printf("Memory not allocated");
    exit(0);
  }
  //loops through the given word and makes a copy
  for(int i=0;i<length;i++){
    copy[i] = word[i];
  }
  //returns the copy of the word.
  return copy;
}

//function to compare two given words. w1-word 1, w2-word 2. Returns true if words are same, false otherwise.
boolean compareWords(char* w1, char* w2){
  //defines l1-word length of w1 and l2- word length of w2, using getWordLength()
  int l1 = getWordLength(w1);
  int l2 = getWordLength(w2);
  //creates copies of the words using copyString.
  char* temp1 = copyString(w1, l1);
  char* temp2 = copyString(w2, l2);
  //converts copies to lowercase using toLower()
  temp1 = toLower(temp1);
  temp2 = toLower(temp2);
  //if l1!=l2 then they are not the same so returns false.
  if(l1!=l2){
    return false;
  }
  else{
    //loops through the words.
    for(int i =0;i<l1;i++){
      //compares each character and if they're not the same returns false
      if(temp1[i]!=temp2[i]){
        //frees the temporary copy variables to prevent memory leak.
        free(temp1);
        free(temp2);
        //returns false
        return false;
      }
    }
    //frees the temporary copy variables to prevent memory leak.
    free(temp1);
    free(temp2);
    //returns true if the entire word has been checked and they were the same.
    return true;
  }
}

//checks to see if the word should be redacted. returns true if it should
boolean isRedacted(char* word, char** redactedWords){
  //loops through redactedWords, comparing each one to the word passed in.
  for(int i =0;i<numberOfRedactedWords;i++){
    //if the words match then it needs to be redacted so return true. uses compareWords()
    if(compareWords(word, redactedWords[i])== true){
      return true;
    }
  }
  //otherwise return false
  return false;
}

//checks to see if the character is a letter. Returns true if no, true otherwise,
boolean checkWordBreaks(char character){
  if(character >64 && character <123){
    return false;
  }
  else{
    return true;
  }
}
//function to generate an array of words to redact
char** generateRedactedWords(file redact){
  //redactedWords - stores words to be redacted 
  char** redactedWords= (char**)malloc(redact.characterLength*sizeof(char*));
  //checks that memory has been allocated
  if(redactedWords == NULL){
    printf("Memory not allocated");
    exit(0);
  }
  //char* word to store each word.
  char *word = (char*)malloc(255);
  //checks that memory has been allocated
  if(word == NULL){
    printf("Memory not allocated");
    exit(0);
  }
  //listCounter - next free-index of redactedWords
  //wordCounter - next free index of word
  int listCounter=0;
  int wordCounter=0;
  //loops through the redact.txt text file and extracts the words
  for(int i=0;i<(redact.characterLength);i++){
    //checks to see if it is a letter, if not then adds current word to redactedWords
    if(checkWordBreaks(redact.text[i])){
      if(word[0] != '\0'){
        word[wordCounter] = '\0'; //lets computer know where the end of the string is -> no random excess characters added
        //adds word to redactedWords
        redactedWords[listCounter] = word;
        //increments listCounter by one
        listCounter++;
        //resets wordCounter
        wordCounter = 0;
        //resets word
        word = (char*)malloc(255);
      }
    }
    //if it is a letter then the word is still being built, adds character to word.
    else{
      word[wordCounter] = redact.text[i];
      //increments wordCounter by one
      wordCounter++;
    }
  }
  //updates numberOfRedactedWords
  numberOfRedactedWords = listCounter;
  //returns redactedWords
  return redactedWords;
}

/**Method to redact words from text
* @param textToRedact is the text that is to be redacted from
* @param redactedWords is the array of words that are to be redacted 
*/
char** generateRedactedText(file textToRedact, char** redactedWords){
  //redactedText - stores new version of text that has been redacted 
  char** redactedText= (char**)malloc(textToRedact.characterLength*sizeof(char*));
  //checks that memory has been allocated
  if(redactedText == NULL){
    printf("Memory not allocated");
    exit(0);
  }
  //stores individual words
  char *word = (char*)malloc(255);
  //checks that memory has been allocated
  if(word == NULL){
    printf("Memory not allocated");
    exit(0);
  }
  //stores non-letter characters
  char *nonLetters = (char*)malloc(2);
  //checks that memory has been allocated
  if(nonLetters == NULL){
    printf("Memory not allocated");
    exit(0);
  }
  //used to temporarily store characters
  char temp[2];
  //counters to keep track of next-free index in word and the redactedText.
  int listCounter=0;
  int wordCounter=0;
  //for loop goes through entire .txt file and redacts words. Also displays the new redacted text.
  for(int i=0;i<(textToRedact.characterLength);i++){
    if(checkWordBreaks(textToRedact.text[i])== true){ //if non-letter
      if(word[0] != '\0'){ //if a word was formed 
        word[wordCounter] = '\0'; //lets computer know where the end of the string is -> no random excess characters added
        if(isRedacted(word, redactedWords) == true){ //if should be redacted.
          for(int i =0; i<wordCounter;i++){
            //replaces redacted word with *
            word[i] = '*';
          }
        }
        //adds word to redacted text
        redactedText[listCounter] = word;
        //displays all words in redacted text
        printf("%s", redactedText[listCounter]);
        listCounter++;
        wordCounter = 0;
        word = (char*)malloc(255);
        word[0] = '\0';
      }
      //temporarily stores characters.
      temp[0] = textToRedact.text[i];
      //sets end-pointer
      temp[1] = '\0';
      redactedText[listCounter] = temp;
      //displays all punctuation in redacted text
      printf("%s", redactedText[listCounter]);
      //increments listCounter by one
      listCounter++;
      //resets nonLetters
      char *nonLetters = (char*)malloc(20);
      }
    //adds character to word, increments wordCounter by one
    else{
      word[wordCounter] = textToRedact.text[i];
      wordCounter++;
    }
  }
  //updates textWordCount
  textWordCount = listCounter;
  //returns redactedText
  return redactedText;
}

int main(void) {
  file textToRedact = getText("debate.txt");
  file redact = getText("redact.txt");
  char** redactedWords = generateRedactedWords(redact);
  char** textWords = generateRedactedText(textToRedact, redactedWords);
  return 0;
}