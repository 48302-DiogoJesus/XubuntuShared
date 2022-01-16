#ifndef WAVE_PLAYLIST
#define WAVE_PLAYLIST

/* ---------- PLAY WAVE ---------- */

int play(Wave* wave);

/* ---------- FILE SEARCH UTILS ---------- */

#define MAX_FILES 500
#define MAX_FILE_NAME_SIZE 100

// Global array that will hold the filepaths of all the files found matching the pattern
char *filepaths[MAX_FILES];

void file_tree_find_wavs(const char *dirpath);
void sort_file_search_results();
void file_show_search_results();                      
static size_t filesFound();

/* ---------- PLAYLIST ---------- */

// Queue Item (Linked list)
typedef struct queueItem {
    struct queueItem *next, *prev;
    Wave *wave;
} QueueItem;

// PLaylist Structure
typedef struct playlist {
    size_t size;
    // Doubly linked list
    struct queueItem *head;
} Playlist;

Playlist *playlist_init();
Wave *playlist_first(Playlist *playlist);
size_t playlist_size(Playlist *playlist);
int playlist_add(Playlist *playlist, Wave *wave);
int playlist_remove(Playlist *playlist, size_t index);
int playlist_wipe(Playlist *playlist);
void playlist_destroy(Playlist *playlist);
void playlist_print(Playlist *playlist);
int playlist_has_file(Playlist *playlist, const char* filename);
void nice_print(const char* message);

/* ---------- COMMANDS ---------- */

typedef struct command {
    char *instruction;
    char *args;
} Command;

#define MAX_COMMANDS_CACHE 100
#define MAX_INPUT_SIZE 100

int *command_get_char();
char *command_wait_valid_input(const char *pre_message);
Command *command_wait(const char *pre_message);
void command_execute(Playlist *playlist, Command *command);
void command_handle_result(int result);
// Individual command functions
void command_help();
void command_scan(char *args);
void command_add(char *args, Playlist *playlist);
void command_remove(char *args, Playlist *playlist);
void command_play(Playlist *playlist);
void commands_free_history();
int commands_history_size();
void command_exit(Playlist *playlist);

char *commands_history[MAX_COMMANDS_CACHE];

/* FUNCTIONALITY REQUIRED TO ALLOW AND RESPOND TO USER INPUT WHILE PLAYING A WAVE FILE */
struct termios orig_termios;

enum WAVE {
	WAVE_PAUSE = 1,
	WAVE_NEXT
};

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}

int termius_get_char()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}

#endif