#include <stdio.h>
/*#include <conio.h>*/
#include <stdlib.h>
#include <string.h>
#include "tong.h"


struct
{
  char name[16];    /* name of player */
  int  stat;        /* active player when is set to 1) */
  int  score;       /* score of the current game */
  int  total;       /* total score since the game is played */
  int  win;         /* point win from last game */
}P[MAXPLAYER];


void gotoxy(int x,int y)
{
    printf("%c[%d;%df",0x1B,y,x);
}


/**********************************************************
 * This function is to get the yes, no and correction
 * input from user
 **********************************************************/
int
getans()
{
  int data;

  do {
    data = getchar();
  }while ((data!='y') && (data!='n') && (data!='c'));

  putchar(data);
  return(data);
}


/*********************************************************
 * This function will get the decemal number form the user
 * This number will be return to the caller function.
 *********************************************************/
int
getnums()
{
  char num[128];
  int value;

  do {
    fgets(num, 32, stdin);
  }while (!isdigit(num));


  value = atoi(num);

  return (value);
}


/*********************************************************
 * This function will display the inactive player or the
 * player has been quitted.
 *********************************************************/
void
display_inactive_players()
{
  int i,j;

  /* clrscr();*/
  system ("clear");

  gotoxy (30,4);
  printf ("These players were quit earlier");

  for (j=0,i=1; i<=Num_player; i++)
  {
    if (P[i].stat)
      continue;

    gotoxy(21,8+j);
    printf("%i. %-10s\n",i,P[i].name);
    j++;
  }

}


/************************************************************************
 * This function will display current puk score and total score of the
 * active player.
 ************************************************************************/
void display_score()
{

   int i,j;

   /* clrscr(); */
   system ("clear");

   gotoxy (30,2);
   printf ("THE TONG MONSTERS");
   gotoxy (32,3);
   printf ("Bet is $%4.2f\n",Bet);
   gotoxy (24,6);
   printf ("NAME         SCORE      TOTAL SCORE\n");

   for (j=0,i=1; i<=Num_player; i++)
   {
      if (P[i].stat == 0)
	continue;

      gotoxy(21,8+j);
      printf("%i. %-10s    %+4d        %+4d\n",i,P[i].name,P[i].score-MAXSCORE,P[i].total);
      j++;
   }

} /* end of display_score() */

/*
 * This function is to save the current score.  This is just incase that
 * we have power lost or accidental of some reason we will have the data
 * store in a file
 */
void
save_current_score()
{
  int i;
  FILE *dfile;
  char *filename;

  filename = "cscore.txt";

  dfile = fopen (filename,"w");
  if (dfile == NULL)
  {
     display_score();
     gotoxy(24,2);
     printf ("Can't open cscore.txt file \n");
     exit(2);
  }

  for (i = 1; i <= Num_player; i++)
     fprintf (dfile,"%s=%d\n",P[i].name,P[i].score);

  fclose(dfile);
} /* endof save_cur_game() */

/*
 * This function is to save the total game score in a file.
 */
void
save_total_score()
{
   int i;
   FILE *dfile;
   char *filename;

   filename = "tscore.txt";
   dfile = fopen(filename,"w");
   if (dfile == NULL)
   {
      display_score();
      gotoxy(24,2);
      printf ("Can't open tscore.txt file\n");
      exit(2);
   }

   for (i = 1; i <= Num_player; i++)
      fprintf (dfile,"%s=%d\n",P[i].name,P[i].total);

   fclose(dfile);
} /* end savedata() */


void
save_game_player_status()
{
  int i;
  FILE *dfile;
  char *filename;

  filename = "status.txt";
  dfile = fopen(filename,"w");
  if (dfile == NULL)
  {
    display_score();
    gotoxy(24,2);
    printf ("Can't open %s file\n",filename);
    exit(2);
  }

  /* save game status */
  fprintf (dfile,"Bet=%f\n",Bet);
  fprintf (dfile,"puk=%d\n",Num_puk);
  fprintf (dfile,"winner=%d\n",Winner);
  fprintf (dfile,"point=%d\n",Point);


  /* save player status*/
  for (i = 1; i <= Num_player; i++)
     fprintf (dfile,"%s=%d\n",P[i].name,P[i].stat);


  fclose(dfile);
}


void
save_puk_score()
{
   int i;
   FILE *dfile;
   char *filename;

   filename = "pscore.txt";
   dfile = fopen(filename,"a");
   if (dfile == NULL)
   {
      display_score();
      gotoxy(24,2);
      printf ("Can't open %s file\n",filename);
      exit(2);
   }

   /* if this is the new start or add new name we need to print the name */
   if (New_start)
   {
     for (i = 1; i <= Num_player; i++)
     {
       fprintf (dfile,"%8s",P[i].name);
     }

     /* print the carriage return or new line */
     fprintf (dfile,"\n");
   }

   for (i = 1; i <= Num_player; i++)
   {
     fprintf (dfile,"%.3d     ",P[i].score - MAXSCORE);
   }

   /* print the carriage return or new line */
   fprintf (dfile,"\n");

   fclose(dfile);
} /* end savedata() */


int
get_total_score()
{
   FILE *dfile;
   char *s;
   char data[128];
   int i,j;
   int player;
   char *filename;

   /* open file for read */
   filename = "tscore.txt";
   dfile = fopen(filename,"r");
   if (dfile == NULL)
   {
      printf ("Can't open %s file\n",filename);
      return 1;
   }


   /* now collect the player name and points */
   player= 1;
   while (fgets(s,30,dfile) != NULL)
   {
      for (i=0; i<128; i++)
	  data[i] = 0;
      j = 0;

      for (i=0; i<30; i++)
      {
	 if (*(s+i) != '=')
	    (P[player].name)[i] = *(s+i);
	 else
	 {
	    i++;
	    while (*(s+i+j) != '\n')
	    {
	       *(data+j) = *(s+i+j);
	       j++;
	    }
	    P[player].total = atoi(data);
	    break; /* get out the for loop */
	 }
      } /* endfor loop */

      player++;
   } /* endwhile loop */

   /*
      get the number of player.  The number of player is one more than
      the actual because the the increament before the loop end
   */
   Num_player = player - 1;

   fclose (dfile);

   return (0);
}/* end of get_total_score() */



int
get_current_score()
{
   FILE *dfile;
   char *s;
   char data[128];    /* creat 256 byte buffer */
   int i,j;
   int player;
   char *filename;


   /* now collect current score */
   filename = "cscore.txt";
   dfile = fopen(filename,"r");
   if (dfile == NULL)
   {
      printf ("Can't open %s file\n",filename);
      return 1;
   }

   player = 1;
   while (fgets(s,30,dfile) != NULL)
   {
      for (i=0; i<128; i++)
	  data[i] = 0;
      j = 0;

      for (i=0; i<30; i++)
      {
	 if (*(s+i) != '=')
	    continue;
	 else
	 {
	    i++;
	    while (*(s+i+j) != '\n')
	    {
	       *(data+j) = *(s+i+j);
	       j++;
	    }
	    P[player].score = atoi(data);
	    break; /* get out the for loop */
	 }
      } /* endfor loop */

      player++;
   } /* endwhile loop */

   fclose(dfile);

   return (0);
}/* end of get_current_score() */

int
get_game_player_status()
{

   FILE *dfile;
   char *s;
   char data[32];    /* creat 256 byte buffer */
   int i,j;
   int player;
   char *filename;


   /* now collect current score */
   filename = "status.txt";
   dfile = fopen(filename,"r");
   if (dfile == NULL)
   {
      printf ("Can't open %s file\n",filename);
      return 1;
   }


   /* Get bet value from the file */
   fgets(s,30,dfile);

   for (i=0;i<128;i++)
      data[i]=0;

   for (i=0;i<30;i++)
   {
      if (*(s+i) != '=')
	 continue;
      else
      {
	 i++;   /* move i beyond = */
	 j = 0;
	 while (*(s+i+j) != '\n')
	 {
	    *(data+j) = *(s+i+j);
	    j++;
	 }
	 Bet = atof(data);
	 break;
      }
   } /* end for */

   /* Get Puk number  from the file */
   fgets(s,30,dfile);

   for (i=0;i<128;i++)
      data[i]=0;

   for (i=0;i<30;i++)
   {
      if (*(s+i) != '=')
	 continue;
      else
      {
	 i++;   /* move i beyond = */
	 j = 0;
	 while (*(s+i+j) != '\n')
	 {
	    *(data+j) = *(s+i+j);
	    j++;
	 }
	 Num_puk = atof(data);
	 break;
      }
   } /* end for */

   /* Get Last winner from the file */
   fgets(s,30,dfile);

   for (i=0;i<128;i++)
      data[i]=0;

   for (i=0;i<30;i++)
   {
      if (*(s+i) != '=')
	 continue;
      else
      {
	 i++;   /* move i beyond = */
	 j = 0;
	 while (*(s+i+j) != '\n')
	 {
	    *(data+j) = *(s+i+j);
	    j++;
	 }
	 Winner = atof(data);
	 break;
      }
   } /* end for */


   /* Get Last game point value from the file */
   fgets(s,30,dfile);

   for (i=0;i<128;i++)
      data[i]=0;

   for (i=0;i<30;i++)
   {
      if (*(s+i) != '=')
	 continue;
      else
      {
	 i++;   /* move i beyond = */
	 j = 0;
	 while (*(s+i+j) != '\n')
	 {
	    *(data+j) = *(s+i+j);
	    j++;
	 }
	 Point = atof(data);
	 break;
      }
   } /* end for */

   /* Get player status */
   player = 1;
   while (fgets(s,30,dfile) != NULL)
   {
      for (i=0; i<32; i++)
	  data[i] = 0;
      j = 0;

      for (i=0; i<30; i++)
      {
	 if (*(s+i) != '=')
	    continue;
	 else
	 {
	    i++;
	    while (*(s+i+j) != '\n')
	    {
	       *(data+j) = *(s+i+j);
	       j++;
	    }
	    P[player].stat = atoi(data);
	    break; /* get out the for loop */
	 }
      } /* endfor loop */

      player++;
   } /* endwhile loop */

   fclose(dfile);

   return (0);
}



void
get_initial_input()
{

   int i;
   int ans;
   int x_axis, y_axis;
   int rc;
   FILE *dfile;
   char *filename;

   do {
     /* clrscr(); */
     system ("clear");
     gotoxy(30,2);
     printf ("The Tong Monsters\n");
     printf ("\n\n\n");
     printf ("Enter number of player (2-8): \n");
     printf ("Enter the cost per point (1-5): \n");
     printf ("1=$.25 2=$.50 3=$1.00 4=$2.00 5=$3.00\n");

     /* get number of player */
     Num_player = 0;
     gotoxy(32,6);
     Num_player = getnums();
     if ((Num_player <2) || (Num_player>8))
	continue;

     /* get the bet */
     do {
       rc = 0;
       gotoxy(34,7);
       putchar (' ');
       gotoxy(34,7);
       rc = getnums();
     }while ((rc<1) || (rc>5));

     switch (rc)
     {
	case 1:
	   Bet = 0.25;
	  /* Ext = "025"; */
	   break;
	case 2:
	   Bet = 0.50;
	 /*  Ext = "050"; */
	   break;
	case 3:
	   Bet = 1.00;
	/*   Ext = "100";  */
	   break;
	case 4:
	   Bet = 2.00;
	/*   Ext = "200";   */
	   break;
	case 5:
	   Bet = 5.00;
	/*   Ext = "300";  */
	   break;
     }

     /* get the name of each player and initialize their scores */
     y_axis = 10;
     gotoxy(1,y_axis);
     printf ("Enter the player's name... \n");
     for (i=1; i<=Num_player; i++)
     {
       printf (" player %d... \n",i);
       gotoxy(15,y_axis+i);
       fgets(P[i].name,64,stdin);
       P[i].score = MAXSCORE;
       P[i].total = 0;
       P[i].stat = 1;
     }

     y_axis += i+1;
     gotoxy(1,y_axis);
     printf("Are these correct (y/n)? \n");
     do {
	ans = 0;
	gotoxy(27,y_axis);
	putchar(' ');
	gotoxy(27,y_axis);
	ans = getans();
     } while (ans == 'c');
   }while ((ans != 'y') && (ans != 'Y'));

   save_total_score();
   save_current_score();
} /* endof getinput() */



void
del_a_player()
{

   int i, j;
   int quitter;
   int ans;

   /* clrscr(); */
   system ("clear");
   display_score();

   gotoxy (30,16);
   printf ("99. Go back");
   gotoxy(20,20);
   printf ("Enter a QUITTER number: \n");
   do {
      gotoxy(45,20);
      putchar(' ');
      gotoxy (45,20);
      quitter = getnums();

      if (quitter == 99)
	 return;

      /* check for invalid and inactive player */
      if ((quitter <1) || (quitter > Num_player))
	continue;
      else if (!P[quitter].stat)
	continue;

      /* confirm to make sure player really wants to quit */
      do {
	 ans = 0;
	 gotoxy(15,22);
	 printf("Does %s really want to quit (y/n)? ",P[quitter].name);
	 gotoxy(36+sizeof(P[quitter].name),22);
	 ans = getans();
      } while (ans == 'c');

      if (ans == 'n')
	 return;

   } while ((quitter<1) || (quitter>Num_player));

   /* if get here then the player really want to quit.  We only
      move the player to inactive state.  We still save all the data */
   P[quitter].stat = 0;

   save_current_score();
   save_total_score();

} /* endof del_a_player) */



void
add_a_player()
{
   int ans;
   int rc = 1;
   int i,j;
   int find_new_player = 0;
   int new_player = Num_player + 1;
   FILE *ofile;

   /* clrscr(); */
   system ("clear");

   if (Num_player >= MAXPLAYER)
   {
      ans = 0;
      gotoxy (25,15);
      printf ("Sorry! you CANNOT add any more player");
      gotoxy (30,16);
      printf ("Press <<c>> ENTER to continue");
      ans = getans();
      return;
   }

   display_inactive_players();
   gotoxy (10,20);
   printf ("Enter the player's name: \n");
   gotoxy (37,20);
   fgets (P[new_player].name, 64, stdin);

   /* double check with the new player */
   ans = 0;
   gotoxy (10,22);
   printf ("Are you sure you want to add %s ? ",P[new_player].name);
   do {
     ans = getans();
   } while (ans == 'c');

   if (ans == 'n')
   {
      return;
   }


   /* if the new player of is already then use the existing data */
   for (i=1; i<Num_player; i++)
   {

     rc = strcmp(P[new_player].name,P[i].name);
     if(!rc)
     { /* new player played before */
       find_new_player = 1;
       P[i].stat = 1;
       break;
     }
   }

   if (!find_new_player)
   {
     /* initialize some data */
     New_start = 1;
     Num_player++;

     P[Num_player].score = MAXSCORE;
     P[Num_player].total = 0;
     P[Num_player].win = 0;
     P[Num_player].stat = 1;
   }

   /*set flag for not allow to change score */
   Dflag = 1;

   return;
} /* endoff add_a_player() */


/********************************************************************************
 * This is a Main() program
 ********************************************************************************/
main()
{
  int i,j;          /* generic index */
  int rc;           /* generic return code */
  int ans;
  int val;
  int pukover=0;    /* flag for Pok over */
  int last_winner=0;
  int last_point=0;
  FILE *tongfile;

  /* initialize some value */
  Winner = 0;
  Point = 0;
  strcpy (P[0].name,"New_Game");

  /* initialize variable */
  for (i=0;i<MAXPLAYER;i++)
  {
     P[i].score = MAXSCORE;
     P[i].total = 0;
  }

  Dflag = TRUE;
  /* clrscr();*/
  system ("clear");

  /* check for new game */
  printf ("Is this a new game (y/n)? ");
  do {
     ans = 'c';
     ans = getans();
  }while (ans=='c');

  if (ans == 'y')
  {
     printf ("\nAre you sure this is a new game (y/n)? ");
     do {
	ans = 'c';
	ans = getans();
     } while (ans == 'c');
  }

  printf ("\n");

  if (ans=='n')
  { /* this is a continue game */

     /* get the bet and the total scores */
     if (get_total_score())
       exit (3);

     /* get the current score */
     if (get_current_score())
       exit (4);

     /* get the player status */
     if (get_game_player_status())
       exit (5);
  }
  else
  { /* this is a new game */
     New_start = 1;
     get_initial_input();
  }


  do {
     display_score();
     gotoxy (2,6);
     printf ("Puk #%d\n",Num_puk);

     gotoxy (20,16);
     printf ("0. Pause the game,  or 99= Quit the game");


     gotoxy (10,17);
     printf ("80=Add new player; 82=Delete a player; 84=correct last input");

     gotoxy (25,20);
     printf ("Last winner: %-10s   Point: %2d\n",P[Winner].name,Point);
     gotoxy (25, 21);
     printf ("Select the winner: \n");

     gotoxy (25,22);
     printf ("Enter the winning point: \n");
     gotoxy (30,23);
     printf ("0=10, 1=12, 2=14");
     gotoxy (25,24);
     printf ("Confirm the input (y/n)\n");

     /* get the winner */
     gotoxy (50,21);
     val = getnums();

     /* do not allow to del the last score */
     if (Dflag && (val == 84))
	continue;


     /* add new player routine */
     if (val == 80)
     {
       add_a_player();
       continue;
     }

     if (val == 82)
     {
	del_a_player();
	continue;
     }

     /***********************************
      * Delete the last score entered
      * *.win can be lost or win.
      ***********************************/
     if (val == 84)
     {
	Dflag = 1;
	for (i=1; i<=Num_player; i++)
	{
	 if (P[i].stat)
	     P[i].score -= P[i].win;
	}

	/* save this game score */
	save_current_score();
	Winner = last_winner;
	Point = last_point;
	continue;
     }
     /* end delete the last score */

     /* want to pause for now */
     if (val == 0)
     {
       save_current_score();
       save_total_score();
       save_game_player_status();
       break;
     }

     /* Want to end the game */
     if (val == 99)
     {
	/* clrscr (); */
        system ("clear");
	gotoxy (10,15);
	printf ("Are you sure you want to QUIT the game (y/n)? ");

	do {
	  gotoxy (57,15);
	  putchar (' ');
	  gotoxy (57,15);
	  ans = getans();
	}while ((ans != 'y') && (ans != 'n'));

	if (ans == 'n')
	  continue;

	/* save Puk score before total the score */
	save_puk_score();

	for (i=1; i<= Num_player; i++)
	{
	   P[i].total += (P[i].score - MAXSCORE);
	   P[i].score = MAXSCORE;
	}

	/* save the scores */
	save_current_score();
	save_total_score();
	save_game_player_status();
	break;
     }

     /* validate player selection */
     if ((val<1) || (val>Num_player))
	continue;

     /**********************************
      * When get here we have a winner *
      **********************************/

     /* get the winning point */
     do {
       rc = 0;
       Dflag = 0;
       gotoxy (50,22);
       putchar (' ');
       gotoxy(50,22);
       rc = getnums();

       /* check for invalid point */
       if ((rc <   3) ||
	   (rc >  14) ||
	   (rc ==  9) ||
	   (rc == 11) ||
	   (rc == 13))
	 continue;

     } while (rc == 0);

    /* get confirmation */
     gotoxy(50,24);
     ans = getans();
     if ((ans != 'y') && (ans != 'Y'))
	continue;

     last_winner = Winner;
     last_point = Point;
     Winner = val;
     Point = rc;
     P[Winner].win = 0;

     /*********************************/
     /* get the score from the losers */
     /* for winner win is the tota win point */
     /* for looser win is a lost point (number is negative) */
     /*********************************/
     for (i=1; i<=Num_player; i++)
     {
       if (!P[i].stat)
	continue;

	if (i != Winner)
	{
	   if (P[i].score <= Point)
	   { /* some one has lost the game */
	      P[Winner].win += P[i].score;
	      P[i].win = -P[i].score;
	      P[i].score = 0;
	      pukover = 1;
	   }
	   else
	   {
	      P[Winner].win += Point;
	      P[i].score -= Point;
	      P[i].win = -Point;
	   }
	}
     } /* end for */

     P[Winner].score += P[Winner].win;

     /* save this game score */
     save_current_score();

     if (!pukover)
	continue;

     /****************************************
      * if it come here then Puk is over
      ****************************************/
     display_score();

     gotoxy(35,18);
     printf ("PUK OVER!");
     gotoxy(15,20);
     printf ("MAKE SURE YOU VERIFY THE LAST WINNER");
     gotoxy(15,21);
     printf ("Do you want to start the NEW GAME (y/n)? ");
     gotoxy(17,22);
     printf ("y...for starting new Puk.");
     gotoxy(17,23);
     printf ("n...for ending the game.");
     gotoxy(17,24);
     printf ("c...for making correction.");
     gotoxy(15,26);
     printf ("Your decision:  ");
     gotoxy(30,26);
     ans = getans();

     if (ans != 'c')
     {
       /* clrscr (); */
       system ("clear");
       display_score();

       gotoxy (15,20);
       printf ("Your LAST chance.  Make sure this PUK is REALLY over");
       gotoxy (15,21);
       printf ("Do you want to start the NEW PUK (y/n)? ");
       gotoxy (17,22);
       printf ("y...for starting new Puk.");
       gotoxy (17,23);
       printf ("n...for ending the game.");
       gotoxy (17,24);
       printf ("c...for making correction.");
       gotoxy (15,26);
       printf ("Your decision:  ");
       gotoxy (30,26);
       ans = getans();
     }

     /**************************************************
      * This is to give a user the last chance to make
      * change after Puk is over.
      **************************************************/
     if (ans == 'c')
     {
	Dflag = 1;
	pukover = 0;

	for (i=1; i<=Num_player; i++)
	{
	   if (P[i].stat)
	      P[i].score -= P[i].win;
	}

	Point = last_point;
	Winner = last_winner;

	/* save the current data */
	save_current_score();
	save_game_player_status();

	/* continue with this Puk */
	continue;
     }

     /* Save the game score */
     /* before total the score */
     save_puk_score();


     /* Reinitialize Score */
     for (i=1; i<=Num_player; i++)
     {
       P[i].total += P[i].score - MAXSCORE;
       P[i].score = MAXSCORE;
     }

     /* save the scores */
     save_current_score();
     save_total_score();
     save_game_player_status();

     if ((ans == 'y') || (ans == 'Y'))
     { /* want another Puk */
	pukover = 0;
	Dflag = 1;
	++Num_puk;
	continue;
     }
     else
	break;

  } while (Winner != 99);


  /* Print the message if the tong game is over */
  /*clrscr(); */
  system ("clear");
  gotoxy (30,1);
  printf ("THE TONG MONSTERS");
  gotoxy (24,3);
  printf ("NAME          SCORE    LOSS/GAIN\n");

  for (i=1; i<=Num_player; i++)
  {
     gotoxy(21,4+i);
     printf("%i. %-10s    %+4d      $%+7.2f",i,P[i].name,P[i].total,P[i].total*Bet);
  }

  return;
} /* end of main */
