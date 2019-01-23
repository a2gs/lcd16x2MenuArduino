/* Andre Augusto Giannotti Scota
 * andre.scota@gmail.com
 * 
 * A menu API for arduino for 16x2 LCD
 */

#include <LiquidCrystal.h>
#include <string.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
// ------------------------------------------------------------------------
typedef struct _menu_t{
  char const **menu;
  int opt;
  char prompt[5];
}menu_t;


/*
        0               1               2
	+----------+    +----------+    +----------+
	| 0   Opc1 |    | 2   Opc3 |    | 4   Opc5 |
	| 1   Opc2 |    | 3   Opc4 |    |          |
	+----------+    +----------+    +----------+

        0               1
	+----------+    +----------+
	| 0   Opc1 |    | 3   Opc4 |
	| 1   Opc2 |    | 4   Opc5 |
	| 2   Opc3 |    |          |
	+----------+    +----------+

        0               1               2
	+----------+    +----------+    +----------+
	| 0   Opc1 |    | 4   Opc5 |    | 8   Opc9 |
	| 1   Opc2 |    | 5   Opc6 |    |          |
	| 2   Opc3 |    | 6   Opc7 |    |          |
	| 3   Opc4 |    | 7   Opc8 |    |          |
	+----------+    +----------+    +----------+

 */

#define MENU_UP (0)
#define MENU_DOWN (1)
#define MENU_ENTER (2)
#define MENU_CANCEL (3)

int run_menu(menu_t *menu, int up_iport, int down_iport, int enter_iport, int cancel_iport, int max_lcd_row, int start_row, LiquidCrystal olcd)
{
  int page;
  int firstPageElement;
  int i;
  int pressed;
  int totalOpts;
  char printText[100] = {0};
  char promptWhiteSpace[50] = {0};

  memset(promptWhiteSpace, 0, 50);
  memset(promptWhiteSpace, ' ', strlen(menu->prompt));

  for(totalOpts = 0; menu->menu[totalOpts] != NULL; totalOpts++);
  
  while(1){
    
    page = menu->opt / max_lcd_row; // start page

    for(i = 0; i < max_lcd_row; i++){ // clear de 'display page'
      olcd.setCursor(0, i + start_row);
      olcd.print("                                                      "); // TODO
    }
      

    for(i = 0; (i < max_lcd_row) && (menu->menu[i][0] != '\0') && (menu->opt < totalOpts); i++){
      
      if(i == menu->opt % max_lcd_row){
        strcpy(printText, menu->prompt);
      }else{
        strcpy(printText, promptWhiteSpace);
      }

      strcat(printText, menu->menu[i + (page * max_lcd_row)]);
      
      olcd.setCursor(0, i + start_row);
      olcd.print(printText);
    }

    do{
      delay(190);
      if(digitalRead(up_iport) == LOW){
        pressed = MENU_UP;
        break;
      }else if(digitalRead(down_iport) == LOW){
        pressed = MENU_DOWN;
        break;
      }else if(digitalRead(enter_iport) == LOW){
        pressed = MENU_ENTER;
        break;
      }else if(digitalRead(cancel_iport) == LOW){
        pressed = MENU_CANCEL;
        break;
      }
    }while(1);

    switch(pressed){
      case MENU_UP:
        if(menu->opt > 0)
          menu->opt--;
        break;
      case MENU_DOWN:
        if(menu->opt < totalOpts)
          menu->opt++;
        break;
      case MENU_ENTER:
        return(menu->opt);
        break;
      case MENU_CANCEL:
        return(-1);
        break;
    }
  }
}
// ------------------------------------------------------------------------

menu_t menu1;

char const *menu_txt[] = {"opcao 1", "opcao 2", "opcao 3", "opcao 4", "opcao 5", "opcao 6", "opcao 7", "opcao 8", NULL};

int state0, state1, state2, state3, state4, state5, state6, state7;

void setup()
{
  lcd.begin(16, 2);
  menu1.menu = menu_txt;
  menu1.opt = 0;
  strcpy(menu1.prompt, "> ");

  pinMode(A0, INPUT); // Up
  pinMode(A1, INPUT); // Down
  pinMode(A2, INPUT); // Enter
  pinMode(A3, INPUT); // Cancel

  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);

  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);

  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(A4, LOW);
  digitalWrite(A5, LOW);
  digitalWrite(13, LOW);

  state0 = LOW;
  state1 = LOW;
  state2 = LOW;
  state3 = LOW;
  state4 = LOW;
  state5 = LOW;
  state6 = LOW;
  state7 = LOW;
}

void loop()
{
  int opt;
    
  //opt = run_menu(&menu1, A0, A1, A2, A3, 1, 0, lcd);
  opt = run_menu(&menu1, A0, A1, A2, A3, 2, 0, lcd);

  switch(opt){
    case 0:
      state0 = !state0;
      digitalWrite(13, state0);
      break;
    case 1:
      state1 = !state1;
      digitalWrite(7, state1);
      break;
    case 2:
      state2 = !state2;
      digitalWrite(8, state2);
      break;
    case 3:
      state3 = !state3;
      digitalWrite(9, state3);
      break;
    case 4:
      state4 = !state4;
      digitalWrite(10, state4);
      break;
    case 5:
      state5 = !state5;
      digitalWrite(A4, state4);
      break;
    case 6:
      state6 = !state5;
      digitalWrite(A5, state4);
      break;
    case 7:
      state7 = !state5;
      digitalWrite(6, state4);
      break;
  }
}
