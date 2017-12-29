#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DEFAULT_BUFFER_SIZE 256
#define TOTAL_CHARS 94
#define CHAR_OFFSET 32

char * getUserInput(int bufferSize);

char * getEncryptedString(char * cstring, const char * key);

char * getDecryptedString(char * encrypted, const char * key);

const char * passkey = "teslakey";

int main(int argc, char * argv[])
{
  char * userInput, * cryptOutput, * decrypted;

  userInput = getUserInput(DEFAULT_BUFFER_SIZE);

  cryptOutput = getEncryptedString(userInput, passkey);

  decrypted = getDecryptedString(cryptOutput, passkey);

  printf("Your input:\n%sEncrypted output:\n%sDecrypted:\n%s\n",userInput,cryptOutput,decrypted);

  for(int i = 0; i < 10; i++) printf("[%d]",decrypted[i]); putchar('\n');

  free(userInput); userInput = NULL;
  free(cryptOutput); cryptOutput = NULL;
  free(decrypted); decrypted = NULL;

  return 0;
}

char * getUserInput(int bufferSize)
{
  char * input, * open;
  int usedBuffer;

  input = (char *)malloc(sizeof(char) * bufferSize);
  if(input == NULL)
  {
    fprintf(stderr,"[ERROR] Unable to alloc input buffer. Killing self.\n");
    exit(1);
  }
  
  // open is a pointer to the next open spot in the input buffer
  open = input;
  usedBuffer = 0;
  
  printf("Enter some text (%d char max):\n", bufferSize);
  while((*open++ = fgetc(stdin)) != '\n' && ++usedBuffer < bufferSize - 1); // -1 to allow space for '\0'
  *open = '\0';
  printf("input: %s\n",input);
  
  return input;
}
  
char * getEncryptedString(char * cstring, const char * key)
{
  int length,keySum,keyLength,swingSum,modifier,i;
  char * output;

  keyLength = 0;
  while(key[keyLength++] != '\0');

  length = 0;
  while(cstring[length++] != '\0');
  
  keySum = 0;
  for(i = 0; key[i] != '\0'; i++)
  {
    keySum += key[i];
  }
  keySum = (keySum % TOTAL_CHARS) + 1;

  output = (char *)malloc(sizeof(char) * length);
  if(output == NULL)
  {
    fprintf(stderr,"[ERROR] Unable to alloc output buffer. Killing self.\n");
    exit(1);
  }

  swingSum = keySum;
  modifier = 1;

  for(i = 0; i < length; i++)
  {
    modifier = ((modifier + 1) % keyLength) + 1;
    if(isprint(cstring[i]))
    {
      swingSum = (swingSum + ((cstring[i] - CHAR_OFFSET) * modifier)) % TOTAL_CHARS + 1;
      output[i] = swingSum + CHAR_OFFSET;
      
    }
    else
    {
      output[i] = cstring[i];
    }
  }

  return output;
}

char * getDecryptedString(char * encrypted, const char * key)
{
  int length, keySum, keyLength, swingSum, modifier, i;
  char * decrypted;

  keyLength = 0;
  while(key[keyLength++] != '\0');

  length = 0;
  while(encrypted[length++] != '\0');

  decrypted = (char *)malloc(sizeof(char) * length);

  keySum = 0;
  for(i = 0; key[i] != '\0'; i++)
  {
    keySum += key[i];
  }
  keySum = (keySum % TOTAL_CHARS) + 1;
  swingSum = keySum;

  printf("DECRYPTION:\n");

  for(i = 0; i < length; i++)
  {
    modifier = ((modifier + 1) % keyLength) + 1;
    if(isprint(encrypted[i]))
    {
      putchar(encrypted[i]);
      printf("encrypted(%d) - ss(%d) =%d, modifier = %d\n",encrypted[i],swingSum,encrypted[i] - swingSum, modifier);
      decrypted[i] = (encrypted[i] - swingSum) / modifier + CHAR_OFFSET;

      swingSum = (swingSum + ((decrypted[i] - CHAR_OFFSET) * modifier)) % TOTAL_CHARS + 1;

      //printf(":ss=%d&out=[%d]\n",swingSum,decrypted[i]);
    }
    else
    {
      decrypted[i] = encrypted[i];
    }
  } 
  putchar('\n');
  return decrypted;  
}
