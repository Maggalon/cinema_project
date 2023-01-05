#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char name[55];
    char year[6];
    char country[20];
    char genres[50];
    char rating[5];
} Film;

typedef struct {
    char login[20];
    char password[20];
    char card_number[17];
    int favorites_list_size;
    int is_admin;
} User;

typedef struct node {
    User user;
    struct node *next;
} User_list;

typedef struct node_t {
    Film film;
    struct node_t *next;
} Favorites;

typedef struct node_f {
    Film film;
    struct node_f *next;
    struct node_f *prev;
} FilmList;

void getFilePath(User user, char *url) {
    char dir[29] = "..\\files\\favorites\\favorites_";
    char extension[4] = ".txt";
    int len = 0, k = 0;
    while (user.login[k] != '\0') {
        len++;
        k++;
    }
    //char url[34+len];
    int i = 0, j = 0;
    while (i < 29) {
        url[i] = dir[i];
        i++;
    }
    while (user.login[j] != '\0') {
        url[i] = user.login[j];
        i++;
        j++;
    }
    j = 0;
    while (j < 4) {
        url[i] = extension[j];
        i++;
        j++;
    }
    url[i] = '\0';
    //return url;
}

int checkName(char *name1, char *name2) {
    int i = 0;
    while (name1[i] != '\0' && name2[i] != '\0') {
        if (name1[i] != name2[i]) {
            return 0;
        }
        i++;
    }
    return 1;
}

//---Работа со списком пользователей-------------
void addUser(User_list **head, User user) {
    if (*head == NULL) {
        *head = (User_list *) malloc(sizeof(User_list));
        (*head)->user = user;
        (*head)->next = NULL;
    } else {
        User_list *cur = *head;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = (User_list *) malloc(sizeof(User_list));
        cur->next->user = user;
        cur->next->next = NULL;
    }
}

void getUsers(User_list **head) {
    User user;
    char s[56];
    FILE *f = fopen("..\\files\\users.txt", "r");
    while (fgets(s, 56, f) != NULL) {
        int j = 0, k = 0, l = 0, i = 0, t = 0;
        char size[2];
        while (s[j] != ' ') {
            user.login[j] = s[j];
            j++;
        }
        user.login[j] = '\0';
        j++;
        while (s[j] != ' ') {
            user.password[k] = s[j];
            j++;
            k++;
        }
        user.password[k] = '\0';
        j++;
        while (s[j] != ' ') {
            user.card_number[l] = s[j];
            j++;
            l++;
        }
        user.card_number[l] = '\0';
        j++;
        while (s[j] != ' ') {
            size[i] = s[j];
            j++;
            i++;
        }
        user.favorites_list_size = strtol(size, (char*) NULL, 10);
        j++;
        user.is_admin = strtol(&s[j], (char*) NULL, 10);
        addUser(head, user);
    }
}

void pushUsers(User_list *head) {
    User_list *cur = head;
    FILE *f = fopen("..\\files\\users.txt", "w");
    while (cur != NULL) {
        fprintf(f, "%s %s %s %d %d\n", cur->user.login, cur->user.password, cur->user.card_number, cur->user.favorites_list_size, cur->user.is_admin);
        cur = cur->next;
    }
}
//------------------------------------------------

//---Работа со списком избранного-----------------
void addFilm(Favorites **head, Film film) {
    if (*head == NULL) {
        *head = (Favorites *) malloc(sizeof(Favorites));
        (*head)->film = film;
        (*head)->next = NULL;
    } else {
        Favorites *cur = *head;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = (Favorites *) malloc(sizeof(Favorites));
        cur->next->film = film;
        cur->next->next = NULL;
    }
}

void deleteFilm(Favorites **head, char *name) {
    if (checkName((*head)->film.name, name)) {
        Favorites *next = (*head)->next;
        free(*head);
        *head = next;
        return;
    }
    Favorites *cur = *head;
    while (!checkName(cur->next->film.name, name)) {
        cur = cur->next;
    }
    Favorites *tmp = cur->next->next;
    free(cur->next);
    cur->next = tmp;
}

void getFavorites(Favorites **head, User user) {
    Film film;
    char url[54];
    getFilePath(user, url);
    //char s[50];
    FILE *f = fopen(url, "r");
    int i = 0;
    while (i < user.favorites_list_size) {
        fgets(film.name, 56, f);
        fgets(film.year, 6, f);
        fgets(film.country, 20, f);
        fgets(film.genres, 50, f);
        fgets(film.rating, 5, f);
        addFilm(head, film);
        i++;
    }
    fclose(f);
    remove(url);
}

int isFilmInList(Favorites *head, char *name) {
    Favorites *cur = head;
    while (cur != NULL) {
        if (checkName(cur->film.name, name)) {
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

void pushFavorites(Favorites *head, User user) {
    Favorites *cur = head;
    char url[54];
    getFilePath(user, url);
    FILE *f = fopen(url, "w");
    while (cur != NULL) {
        fprintf(f, "%s%s%s%s%s", cur->film.name, cur->film.year, cur->film.country, cur->film.genres, cur->film.rating);
        cur = cur->next;
    }
    fclose(f);
}
//------------------------------------------------

int isValid(char *str, char type) {
    int i = 0;
    if (type == 'l') {
        while (str[i] != '\0') {
            if (str[i] >= 48 && str[i] <= 57 || str[i] >= 65 && str[i] <= 90 || str[i] >= 97 && str[i] <= 122) {
                i++;
            } else {
                return 0;
            }
        }
        if (i < 3 || i > 20) {
            return 0;
        }
        return 1;
    } else if (type == 'p') {
        int haveUpper = 0, haveLower = 0, haveDigit = 0;
        while (str[i] != '\0') {
            if (str[i] >= 48 && str[i] <= 57 || str[i] >= 65 && str[i] <= 90 || str[i] >= 97 && str[i] <= 122) {
                if (str[i] >= 65 && str[i] <= 90) haveUpper = 1;
                if (str[i] >= 97 && str[i] <= 122) haveLower = 1;
                if (str[i] >= 48 && str[i] <= 57) haveDigit = 1;
                i++;
            } else {
                return 0;
            }
        }
        if (i >= 6 && i <= 20 && haveUpper && haveDigit && haveLower) {
            return 1;
        }
        return 0;
    } else {
        while (str[i] != '\0') {
            if (str[i] < 48 || str[i] > 57) return 0;
            i++;
        }
        if (i == 16) return 1;
        return 0;
    }
}

int checkLogin(char *login, char type, User_list *head) {
    User_list *cur = head;
    while (cur != NULL) {
        int i = 0, ok = 1;
        while (cur->user.login[i] != '\0') {
            if (login[i] != cur->user.login[i]) {
                ok = 0;
                break;
            }
            i++;
        }
        if (ok) {
            if (type == 'r') printf("Login %s is used. Try another one.\n", login);
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

int checkPassword(char *login, char *password, char *card_number, int *size, int *is_admin, User_list *head) {
    User_list *cur = head;
    while (cur != NULL) {
        int i = 0, ok = 1;
        while (cur->user.login[i] != '\0') {
            if (cur->user.login[i] != login[i]) {
                ok = 0;
                break;
            }
            i++;
        }
        i = 0;
        if (ok) {
            while (cur->user.password[i] != '\0') {
                if (cur->user.password[i] != password[i]) {
                    return 0;
                }
                i++;
            }
            i = 0;
            while (i < 16) {
                card_number[i] = cur->user.card_number[i];
                i++;
            }
            *size = cur->user.favorites_list_size;
            *is_admin = cur->user.is_admin;
            return 1;
        }
        cur = cur->next;
    }
}

void registration(char *login, char *password, char *card_number, User_list *head) {
    while (1) {
        printf("Login (3-20 letters and digits):\n");
        scanf("%s", login);
        if (checkLogin(login, 'r', head)) continue;

        if (!isValid(login, 'l')) {
            printf("Invalid login. Try again.\n");
            continue;
        }
        break;
    }
    while (1) {
        printf("Password (6-20 letters and digits, 1 upper/lowercase letter and digit are required):\n");
        scanf("%s", password);
        if (!isValid(password, 'p')) {
            printf("Invalid password. Try again.\n");
            continue;
        }
        break;
    }
    while (1) {
        printf("Card number (16 digits):\n");
        scanf("%s", card_number);
        if (!isValid(card_number, 'c')) {
            printf("Invalid card number. Try again.\n");
            continue;
        }
        break;
    }
}

FilmList *init(Film film) {
    FilmList *lst = (FilmList*) malloc(sizeof(FilmList));
    lst->film = film;
    lst->next = lst;
    lst->prev = lst;
    return lst;
}

FilmList *addToFilmList(FilmList *lst, Film film) {
    FilmList *tmp, *p;
    tmp = (FilmList*) malloc(sizeof(FilmList));
    p = lst->next;
    lst->next = tmp;
    tmp->film = film;
    tmp->next = p;
    tmp->prev = lst;
    p->prev = tmp;
    return tmp;
}

FilmList *deleteFromFilmList(FilmList *lst) {
    FilmList *prev, *next;
    prev = lst->prev;
    next = lst->next;
    prev->next = lst->next;
    next->prev = lst->prev;
    free(lst);
    return prev;
}

void printCards(FilmList *cur){
    printf("                                       ╔═════════════════════════════════════════════════════╗ \n");
    printf("                                       ║                                                     ║ \n");
    printf("                                       ║                                                     ║ \n");
    printf("┌────────────────────────────────────┐ ║                                                     ║ ┌────────────────────────────────────┐  \n");
    printf("│                                    │ ║                                                     ║ │                                    │ \n");
    printf("│                                    │ ║                                                     ║ │                                    │ \n");
    printf("│                                    │ ║                                                     ║ │                                    │ \n");
    printf("│                                    │ ║                                                     ║ │                                    │ \n");
    printf("│                                    │ ║                                                     ║ │                                    │ \n");
    printf("│                                    │ ║                                                     ║ │                                    │ \n");
    printf("│                                    │ ║                                                     ║ │                                    │\n");
    printf("│                                    │ ║                                                     ║ │                                    │\n");
    printf("│                                    │ ║                                                     ║ │                                    │\n");
    printf("│                                    │ ║                                                     ║ │                                    │\n");
    printf("│                                    │ ║                                                     ║ │                                    │\n");
    printf("└────────────────────────────────────┘ ║                                                     ║ └────────────────────────────────────┘ \n");
    printf("                                       ║                                                     ║ \n");
    printf("                                       ║                                                     ║ \n");
    printf("                                       ╚═════════════════════════════════════════════════════╝ \n");

}

int main() {

    User_list *head = NULL;
    getUsers(&head);

    char option;
    printf("Sup! Welcome to the film catalog.\n");
    printf("If you are a new user, please type 'r' to register.\n");
    printf("If you have already registered, please type 'l' to log in.\n");
    scanf("%c", &option);

    User user;
    FILE *user_favorites;

    if (option == 'r') {
        registration(user.login, user.password, user.card_number, head);
        user.favorites_list_size = 0;
        user.is_admin = 0;
        addUser(&head, user);

        char url[54];
        getFilePath(user, url);
        user_favorites = fopen(url, "w");
    }
    else if (option == 'l') {
        while (1) {
            printf("Login:\n");
            scanf("%s", user.login);
            if (!checkLogin(user.login, 'l', head)) {
                printf("There is no such user. Try again.\n");
                continue;
            }
            break;
        }
        while (1) {
            printf("Password:\n");
            scanf("%s", user.password);
            if (!checkPassword(user.login, user.password, user.card_number, &user.favorites_list_size, &user.is_admin, head)) {
                printf("Wrong password. Try again\n");
                continue;
            }
            break;
        }
    }
    else {
        printf("Nothing to do with this input.");
        return 0;
    }


    while (1) {
        printf("a/d to control\nD to see detailed info\nA to add film to your favorites\nF to see your favorites\nc to change user data\n");
        if (user.is_admin) {
            printf("@ to add new film to catalog\n& to delete film from catalog\n");
        }
        printf("f to finish work:");
        scanf("\n%c", &option);
        if (option == 'f') {
            printf("Thanks for using our services.");
            break;
        } else if (option == 'c') {
            while (1) {
                system("cls");
                printf("To exit profile settings type 'e'.\n");
                printf("To change login/password/card number type 'l'/'p'/'c':\n");
                scanf("\n%c", &option);
                if (option == 'e') break;
                if (option == 'l') {
                    User_list *cur = head;
                    while (cur != NULL) {
                        int i = 0, ok = 1;
                        while (cur->user.login[i] != '\0') {
                            if (cur->user.login[i] != user.login[i]) {
                                ok = 0;
                                break;
                            }
                            i++;
                        }
                        if (ok) break;
                        cur = cur->next;
                    }
                    while (1) {
                        printf("New login (3-20 letters and digits):\n");
                        scanf("%s", user.login);
                        if (checkLogin(user.login, 'r', head)) continue;

                        if (!isValid(user.login, 'l')) {
                            printf("Invalid login. Try again.\n");
                            continue;
                        }
                        break;
                    }
                    int i = 0;
                    while (user.login[i] != '\0') {
                        cur->user.login[i] = user.login[i];
                        i++;
                    }
                    cur->user.login[i] = '\0';
                }
                else if (option == 'p') {
                    User_list *cur = head;
                    while (cur != NULL) {
                        int i = 0, ok = 1;
                        while (cur->user.login[i] != '\0') {
                            if (cur->user.login[i] != user.login[i]) {
                                ok = 0;
                                break;
                            }
                            i++;
                        }
                        if (ok) break;
                        cur = cur->next;
                    }
                    while (1) {
                        printf("New password (6-20 letters and digits, 1 upper/lowercase letter and digit are required):\n");
                        scanf("%s", user.password);
                        if (checkLogin(user.password, 'r', head)) continue;

                        if (!isValid(user.password, 'p')) {
                            printf("Invalid password. Try again.\n");
                            continue;
                        }
                        break;
                    }
                    int i = 0;
                    while (user.password[i] != '\0') {
                        cur->user.password[i] = user.password[i];
                        i++;
                    }
                    cur->user.password[i] = '\0';
                }
                else if (option == 'c') {
                    User_list *cur = head;
                    while (cur != NULL) {
                        int i = 0, ok = 1;
                        while (cur->user.login[i] != '\0') {
                            if (cur->user.login[i] != user.login[i]) {
                                ok = 0;
                                break;
                            }
                            i++;
                        }
                        if (ok) break;
                        cur = cur->next;
                    }
                    while (1) {
                        printf("New card number (16 digits):\n");
                        scanf("%s", user.card_number);
                        if (!isValid(user.card_number, 'c')) {
                            printf("Invalid card number. Try again.\n");
                            continue;
                        }
                        break;
                    }
                    int i = 0;
                    while (i < 16) {
                        cur->user.card_number[i] = user.card_number[i];
                        i++;
                    }
                }
                else printf("Nothing to do with this input.");
            }
        }
    }

    pushUsers(head);
    return 0;
}
