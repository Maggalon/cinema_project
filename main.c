#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

void printUsers(User_list *head) {
    User_list *cur = head;
    while (cur != NULL) {
        printf("%s %s %s\n", cur->user.login, cur->user.password, cur->user.card_number);
        cur = cur->next;
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


typedef struct node_f {
    Film film;
    struct node_f *next;
    struct node_f *prev;
} FilmList;

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

void printFilm(FilmList *cur, int n) {
    if (n) {
        printf("%s", cur->film.name);
        printf("%s", cur->film.year);
        printf("%s", cur->film.country);
        printf("%s", cur->film.genres);
        printf("%s\n", cur->film.rating);
    } else {
        printf("%s", cur->film.name);
        printf("%s\n", cur->film.rating);
    }
}

int isFilmValid(FilmList *lst, char *name) {
    FilmList *cur = lst;
    do {
        if (checkName(cur->film.name, name)) {
            return 0;
        }
        cur = cur->next;
    } while(cur != lst);
    return 1;
}

Film getFilm(FILE *f) {
    Film film;
    fgets(film.name, 56, f);
    fgets(film.year, 6, f);
    fgets(film.country, 20, f);
    fgets(film.genres, 50, f);
    fgets(film.rating, 5, f);
    return film;
}

FilmList *getFilmList(char type, User user) {
    FILE *f;
    int len;
    char size[3];
    if (type == 'f') {
        char url[54];
        getFilePath(user, url);
        f = fopen(url, "r");
        len = user.favorites_list_size - 1;
    } else {
        f = fopen("..\\files\\films_list_size.txt", "r");
        fgets(size, 4, f);
        len = strtol(size, (char*) NULL, 10) - 1;
        fclose(f);
        f = fopen("..\\files\\films.txt", "r");
    }
    Film firstFilm = getFilm(f);
    FilmList *lst = init(firstFilm);
    FilmList *current = lst;
    for (int i = 0; i < len; i++) {
        Film film = getFilm(f);
        current = addToFilmList(current, film);
    }
    fclose(f);
    return lst;
}

void addToFilmsFile(Film film) {
    FILE *f = fopen("..\\files\\films.txt", "a");
    fprintf(f, "%s%s%s%s%s", film.name, film.year, film.country, film.genres, film.rating);
    fclose(f);
    char size[2];
    f = fopen("..\\files\\films_list_size.txt", "r");
    fgets(size, 4, f);
    int len = strtol(size, (char*) NULL, 10) + 1;
    fclose(f);
    f = fopen("..\\files\\films_list_size.txt", "w");
    fprintf(f, "%d", len);
}

void pushFilmList(FilmList *lst, User user, char type) {
    FilmList *cur = lst;
    FILE *f;
    if (type == 'f') {
        char url[54];
        getFilePath(user, url);
        f = fopen(url, "w");
    } else {
        f = fopen("..\\files\\films.txt", "w");
    }
    do {
        fprintf(f, "%s%s%s%s%s", cur->film.name, cur->film.year, cur->film.country, cur->film.genres, cur->film.rating);
        cur = cur->next;
    } while (cur != lst);

}

int countFilmName(FilmList *cur){
    int count = 0;
    while((char)cur->film.name[count] != '\0')
        count += 1;
    return count;
}

int neededSpaces(FilmList *cur){
    int spaces = 53, neededSpaces;
    neededSpaces = spaces / 2 + countFilmName(cur) / 2;
    return neededSpaces;
}

void printCards(FilmList *cur){
    char *name, *rating;
    name = cur->film.name; rating = cur->film.rating;
    name[strcspn(name, "\n")] = 0; rating[strcspn(rating, "\n")] = 0;
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,186,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,186);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,186,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,186);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",218,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,191,32,186,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,186,32,218,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,191);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179,32,186,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,186,32,179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179,32,186,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,186,32,179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179,32,186,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,186,32,179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%*s%*c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179,32,186, neededSpaces(cur), name, 53-neededSpaces(cur),32,186,32,179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179,32,186,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,186,32,179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179,32,186,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,186,32,179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%*s%*c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179,32,186, 27, rating, 53-27, 32,186,32,179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179,32,186,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,186,32,179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179,32,186,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,186,32,179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179,32,186,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,186,32,179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179,32,186,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,186,32,179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,179);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",192,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,217,32,186,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,186,32,192,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,217);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,186,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,186);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,186,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,186);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);
    name[strcspn(name, "\n")] = '\n'; rating[strcspn(rating, "\n")] = '\n';
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

    FilmList *favlst = getFilmList('f', user);
    //printFilmList(favlst);
    Favorites *favorites = NULL;
    getFavorites(&favorites, user);
    //printFavorites(favorites);

    printf("\n\nWelcome, %s\n\n", user.login);

    FilmList *lst = getFilmList(' ', user);
    //printFilmList(lst);

    //char option;
    while (1) {
        system("cls");
        printCards(lst);
//        printFilm(lst->prev, 0);
//        printFilm(lst, 0);
//        printFilm(lst->next, 0);
        printf("a/d to control\nD to see detailed info\nA to add film to your favorites\nF to see your favorites\nc to change user data\n");
        if (user.is_admin) {
            printf("@ to add new film to catalog\n& to delete film from catalog\n");
        }
        printf("f to finish work:");
        scanf("\n%c", &option);
        //system("cls");
        if (option == 'a') {
            lst = lst->prev;
        } else if (option == 'd') {
            lst = lst->next;
        } else if (option == 'D') {
            system("cls");
            printFilm(lst, 1);
            while (1) {
                printf("A to add film to your favorites\n");
                if (user.is_admin) {
                    printf("& to delete film from catalog\n");
                }
                printf("e to get back to the list:");
                scanf("\n%c", &option);
                //system("cls");
                if (option == 'e') {
                    break;
                } else if (option == 'A'){
                    if (isFilmInList(favorites, lst->film.name)) {
                        system("cls");
                        printf("This film is already in your favorites.\n");
                        sleep(2);
                        continue;
                    }
                    addFilm(&favorites, lst->film);
                    if (user.favorites_list_size == 0) {
                        favlst = init(lst->film);
                    } else {
                        addToFilmList(favlst, lst->film);
                    }
                    user.favorites_list_size++;
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
                    cur->user.favorites_list_size++;
                } else if (option == '&') {

                    FilmList *delP = favlst;
                    do {
                        if (checkName(delP->film.name, lst->film.name)) {
                            favlst = deleteFromFilmList(delP);
                            break;
                        }
                        delP = delP->next;
                    } while (delP != favlst);

                    Favorites *cur = favorites;
                    while (cur != NULL) {
                        if (checkName(cur->film.name, lst->film.name)) {
                            deleteFilm(&favorites, cur->film.name);
                            user.favorites_list_size--;
                            User_list *cur_user = head;
                            while (cur_user != NULL) {
                                if (checkName(cur_user->user.login, user.login)) {
                                    cur_user->user.favorites_list_size--;
                                }
                                cur_user = cur_user->next;
                            }
                            break;
                        }
                        cur = cur->next;
                    }
                    //user.favorites_list_size--;

                    User_list *cur_user = head;
                    while (cur_user != NULL) {
                        if (!checkName(cur_user->user.login, user.login)) {
                            FilmList *favP = getFilmList('f', cur_user->user);
                            FilmList *delfavP = favP;
                            do {
                                if (checkName(delfavP->film.name, lst->film.name)) {
                                    favP = deleteFromFilmList(delfavP);
                                    pushFilmList(favP, cur_user->user, 'f');
                                    cur_user->user.favorites_list_size--;
                                }
                                delfavP = delfavP->next;
                            } while (delfavP != favP);
                        }
                        cur_user = cur_user->next;
                    }

                    lst = deleteFromFilmList(lst);
                    char size[2];
                    FILE *f = fopen("..\\files\\films_list_size.txt", "r");
                    fgets(size, 4, f);
                    int len = strtol(size, (char*) NULL, 10) - 1;
                    fclose(f);
                    f = fopen("..\\files\\films_list_size.txt", "w");
                    fprintf(f, "%d", len);
                    fclose(f);
                }
            }
        } else if (option == 'A') {
            if (isFilmInList(favorites, lst->film.name)) {
                system("cls");
                printf("This film is already in your favorites.\n");
                sleep(2);
                continue;
            }
            addFilm(&favorites, lst->film);
            if (user.favorites_list_size == 0) {
                favlst = init(lst->film);
            } else {
                addToFilmList(favlst, lst->film);
            }
            user.favorites_list_size++;
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
            cur->user.favorites_list_size++;
        } else if (option == 'F') {
            if (user.favorites_list_size == 0) {
                printf("You have no favorites yet. Please add some.\n");
                sleep(2);
            } else {
                while (1) {
                    system("cls");
                    if (user.favorites_list_size == 1) {
                        printCards(favlst);
                        //printFilm(favlst, 0);
                    } else {
                        printCards(favlst);
//                        printFilm(favlst->prev, 0);
//                        printFilm(favlst, 0);
//                        printFilm(favlst->next, 0);
                    }
                    printf("a/d to control\nI to see detailed info\nD to delete film from your favorites\ne to exit:");
                    scanf("\n%c", &option);
                    if (option == 'a') {
                        favlst = favlst->prev;
                    } else if (option == 'd') {
                        favlst = favlst->next;
                    } else if (option == 'I') {
                        printFilm(favlst, 1);
                        while (1) {
                            printf("d to delete film from your favorites\ne to get back to the list:");
                            scanf("\n%c", &option);
                            //system("cls");
                            if (option == 'e') {
                                break;
                            } else if (option == 'd') {
                                user.favorites_list_size--;
                                deleteFilm(&favorites, favlst->film.name);
                                favlst = deleteFromFilmList(favlst);
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
                                cur->user.favorites_list_size--;
                                //if (user.favorites_list_size == 0) break;
                            }
                        }
                    } else if (option == 'D') {
                        user.favorites_list_size--;
                        deleteFilm(&favorites, favlst->film.name);
                        favlst = deleteFromFilmList(favlst);
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
                        cur->user.favorites_list_size--;
                        //if (user.favorites_list_size == 0) break;
                    }
                    if (user.favorites_list_size == 0) {
                        printf("Your favorites list is empty. You will be redirected to catalog.");
                        sleep(2);
                        break;
                    }
                    if (option == 'e') {
                        break;
                    }
                }
            }
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
        } else if (option == '@') {
            char tmp;
            scanf("%c", &tmp);
            system("cls");
            Film film;
            printf("Adding new film");
            while (1) {
                printf("\nName:");
                fgets(film.name, 60, stdin);
                if (!isFilmValid(lst, film.name)) {
                    printf("\nList already contains this film\n");
                    continue;
                }
                break;
            }
            printf("Year:");
            fgets(film.year, 10, stdin);
            printf("Country:");
            fgets(film.country, 30, stdin);
            printf("Genres:");
            fgets(film.genres, 60, stdin);
            printf("Rating:");
            fgets(film.rating, 10, stdin);
            addToFilmList(lst, film);
            addToFilmsFile(film);
        } else if (option == '&') {

            FilmList *delP = favlst;
            do {
                if (checkName(delP->film.name, lst->film.name)) {
                    favlst = deleteFromFilmList(delP);
                    break;
                }
                delP = delP->next;
            } while (delP != favlst);

            Favorites *cur = favorites;
            while (cur != NULL) {
                if (checkName(cur->film.name, lst->film.name)) {
                    deleteFilm(&favorites, cur->film.name);
                    user.favorites_list_size--;
                    User_list *cur_user = head;
                    while (cur_user != NULL) {
                        if (checkName(cur_user->user.login, user.login)) {
                            cur_user->user.favorites_list_size--;
                        }
                        cur_user = cur_user->next;
                    }
                    break;
                }
                cur = cur->next;
            }
            //user.favorites_list_size--;

            User_list *cur_user = head;
            while (cur_user != NULL) {
                if (!checkName(cur_user->user.login, user.login)) {
                    FilmList *favP = getFilmList('f', cur_user->user);
                    FilmList *delfavP = favP;
                    do {
                        if (checkName(delfavP->film.name, lst->film.name)) {
                            favP = deleteFromFilmList(delfavP);
                            pushFilmList(favP, cur_user->user, 'f');
                            cur_user->user.favorites_list_size--;
                        }
                        delfavP = delfavP->next;
                    } while (delfavP != favP);
                }
                cur_user = cur_user->next;
            }

            lst = deleteFromFilmList(lst);
            char size[3];
            FILE *f = fopen("..\\files\\films_list_size.txt", "r");
            fgets(size, 4, f);
            int len = strtol(size, (char*) NULL, 10) - 1;
            fclose(f);
            f = fopen("..\\files\\films_list_size.txt", "w");
            fprintf(f, "%d", len);
            fclose(f);
        } else if (option == 'f') {
            printf("Thanks for using our services.");
            sleep(2);
            break;
        }
    }

    pushFilmList(lst, user, ' ');
    pushUsers(head);
    pushFavorites(favorites, user);

    return 0;
}
