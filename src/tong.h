int  getnums (void);
int  getans (void);
void display_score();
void display_score_by_puk();
void save_current_score();
void save_total_score();
void save_game_player_status();
void save_puk_score();
int  get_total_score();
int  get_current_score();
int  get_game_player_status();
void get_initial_input();
void del_a_player();
void add_a_player();
void gotoxy(int x,int y);

#define MAXPLAYER   20
#define MAXSCORE    50
#define TRUE         1
#define FALSE        0


/* Global Variables */
int  Num_puk;
int  New_start;
int  Num_player;
int  Dflag;
char *Ext;
int  Winner;
int  Point;

float Bet;






