//Ian Leuty CS162-001 Program2 April 19, 2024, ileuty@pdx.edu

/*
This program is designed to practice reading comprehension. 2 players will use this program. 
First, a player will enter a sentence and it will be cleared from the screen as soon as they enter it. 
Second, the program will display the sentance to the other player for ~1 second. 
Third, the screen will be cleared again after the time is up. 
Fourth, the second player who just saw the sentance will try to enter it as accurately 
as possible. 
Between these steps, the program will need to convert both strings to lowercase, it will 
also ignore punctuation in some way.
Fifth, the program will compare word by word the first array
entered with the array of character the second player entered and count 
how many matches there are. 
Sixth, output the percentage of correct words.
If they got less than 50% right, re-display the original sentence for some amout of time longer than before. Re-display twice, then end the round (total of 3 allowed attempts)
Lastly, switch player's roles and repeat. 
*/

#include <iostream>
#include <cctype>
#include <cstring> 
#include <unistd.h>

using namespace std;

const int SIZE{400};

void welcome();
void players(char player[], int player_number);
void cap_player(char player[]);
void input_original(char original[], char player[]);
void clear();
void display_time(char original[], float time);
void input_match(char match[], char player[]);
void lower_chars(char chars[]);
void switch_players(char player1[], char player2[]);
void rm_punct(char chars[], char new_word[]);
void extract_word(char phrase[], int & start, char word[]);

float percentmatched(char original[], char match[]);

//main program calling routine
int main(){
    char player1[SIZE],player2[SIZE]; //player name arrays
    char original_phrase[SIZE], match_phrase[SIZE]; //original and match attempt arrays
    char selection{' '}, choice{' '}; //selection to quit, variable to take another attempt
    int p1{1}, p2{2}, attempts{1}, o_words{0}, m_words{0}; //player numbers, attempts
    float match_perc{0}, time{3.0}; //percentage matched words, initial time
    //loop controls - attempt loop, main quit loop, take attempt switch
    bool again {true}, quit{false}, understood{false}; 
        
        clear(); 
        welcome(); //welcome players, get their names and capitalize them 
        players(player1, p1);
        players(player2, p2);
        cap_player(player1);
        cap_player(player2);
 
    while(quit == false){ //loop to start game. option to quit w/ error check
        time = 2.0;
        attempts = 1;
        again = true;


        cout << "\nAre you ready to play? ('y'/'n')\n" << '>';
        cin >> selection;

        //i've been having to do this or i get an infinite loop of re-prompting???
        cin.ignore(100, '\n');

        if(selection == 'Y' || selection == 'y'){  //pick yes
            quit = false;    
            cout << "\nPlayer 1:\n" << player1 << endl; //print player names
            cout << "\nPlayer 2:\n" << player2 << endl;
            input_original(original_phrase, player1); //get the first player's phrase
            understood = false; 
            clear(); 

            while(understood == false){ //keeps clear and timer from occuring until ready
                cout << "\n" << player2 
                     << ", enter anything when you're ready to see the phrase " 
                     << player1 << " typed.\n" << endl;
                cin >> choice;
                cin.ignore(100, '\n'); //again should not be needed...but is ?

                if(choice)
                    understood = true;
            }

            while(again == true && attempts <= 3){ //loop to allow 3 attempts
                clear();   //clear function clears screen
                display_time(original_phrase, time); //timed cout of og phrase
                clear();
                input_match(match_phrase, player2); // get the second player's attempt input
            
                match_perc = percentmatched(original_phrase, match_phrase);  
                                                                
                cout << "\n" << player2 << ", you matched " 
                     << match_perc << "% of the words." << endl; //inform players
                
                if(match_perc >= 50){ //score better than 50%
                    cout << "Nice work, " << player2 << "!\n" 
                         << endl;
                    again = false;
                }

                else if(match_perc < 50 && attempts < 3){ 
                    //score worse than 50% and attempts left

                    cout <<  player2 << ", you didn't enter enough correctly.\n"
                         << "You'll be able to try again.\n"
                         << "The message will show a little longer. \n" << endl;

                    time = (time * 2); //increase time by factor 
                    ++attempts; //increment attempts 
                    understood = false; //reset comfirm switch

                    while(understood == false){ 
                        //keeps clear and timed cout from occuring until p2 ready
                        cout << "\nEnter anything to continue.\n" << endl;
                        cin >> choice;
                        cin.ignore(100, '\n'); //again should not be needed...but is ?

                        if(choice)
                            understood = true;
                    }
                }

                else{ //no more attempts
                    again = false;
                    cout << "Sorry " << player2 << ", you're out of attempts." << endl;
                }
            } 
 
        //notify player swap will occur if they go again
        //end of main game 
        cout << "\nReturning to main menu...\n\n"  
             << "If you choose to play again, you will automatically switch roles.\n" << endl; 
        switch_players(player1, player2);
        } 

        else if(selection == 'n' || selection == 'N'){ //quit
            quit = true;
            cout << "\nBye!\n" << endl;
        }

        else //error check 
            cout << "\nPlease enter 'y' or 'n'." << endl;    
    } 

    return 0;
}


//wecome the player and explain the game
void welcome(){
    cout << "\n\nWelcome to Reading Comprehension Practice!\n\n"
         << "This program allows two players to test one another's ability to read\n"
         << "and retain sentences quickly.\n\n"
         << "Once you enter your names, the program will allow Player 1 to enter a sentence.\n"
         << "Then, the sentence will be dispayed on the screen for a few seconds.\n"
         << "Player 2 will have to enter that sentence in as accurately as possible.\n"
         << "They will be given a score based on percentage of words correct.\n\n"
         << "Player 2 will have 3 tries to score greater than 50%, and will be given a \n"
         << "longer time to read the sentence each time.\n\n"
         << "Then, the players will switch and play again if they choose.\n\n"
         << "The reader will not be scored on punctuation or capitalization.\n" << endl;
}

//get the players names
void players(char player[], int playernum){
    cout << "\nPlayer " << playernum<< ", please enter your name.\n" << '>';
    cin.get(player, SIZE, '\n');
    cin.ignore(10000, '\n');
}

//capialize the players names
void cap_player(char player[]){
    int len{0}, i{0};
    len = strlen(player);
    lower_chars(player);  //just make them lowecase first
    player[0] = toupper(player[0]); //capitalize first letter
    for(i = 0; i < len; ++i){
        if(isblank(player[i])){
            player[i+1] = toupper(player[i+1]); //subsequent names also
        }
    }
}
    

//prompt and read in the first player's sentence
//prompt player by name 
void input_original(char phrase[], char player[]){
    cout << "\n" << player << ", please enter a sentence no longer than " 
         << SIZE << " characters\n"
         << "including punctuation and spaces.\n" << '>';
    cin.get(phrase, SIZE, '\n');
    while(cin.peek() != '\n'){
        cout << "Character limit exceeded. Please enter less than " 
             << SIZE << " characters.\n" << '>';
        cin.ignore(100000, '\n');
        cin.get(phrase, SIZE, '\n');
    }
    cin.ignore(10000, '\n');
}

//prompt and read in the second player's attempt
//also include their name
void input_match(char match[], char player[]){
    cout << player << ", please enter the sentence that was just displayed.\n" << '>';
    cin.get(match, SIZE, '\n'); 
    while(cin.peek() != '\n'){
        cout << "Character limit exceeded. Please enter less than " 
             << SIZE << " characters.\n" << '>';
        cin.ignore(100000, '\n');
        cin.get(match, SIZE, '\n');
    }

    cin.ignore(10000, '\n');
}

//display a phrase for a set amount of time. takes the phrase and time from main as an argument
//time and phrase can be set in main
void display_time(char phrase[], float time){
    cout << phrase << "\n" << endl;
    sleep(time);
}

//clear the screen (may only work on linux w/ g++ compliler)
void clear(){
    cout << "\033[2J\033[1;1H" << endl;
}

//separate the phrase into words base on spaces. returns index position after the found space
//takes a phrase, a starting index, and a blank array to hold the phrase
//pass the value of the supposed index of the next word by refrence 
void extract_word(char phrase[], int & start, char pulled[]){ 
    char word[SIZE]; //word array to start working with
    int len_phrase{0}; //length of the phrase
    int len_word{0}; //empty for now length of word
    int space{0}; //index locaton of the space
    int z{0}, u{start}; //separate variable to increment the word array we're pulling in to
                        //'u' grabs the starting index so it can be incremented while preserving that position
    len_phrase = strlen(phrase);
    //this runs the array until it hits a space or terminating character 
    if(isblank(phrase[start]) == true)
        ++start;
    for(u = start; !isblank(phrase[u]) && phrase[u] != '\0'; ++u){
        word[z] = phrase[u];
        word[z+1] = '\0'; //always adds teminating character to the next position. overwritten until we hit the end of the word
        ++z;
    }
    
    len_word = strlen(word);
    start = start + len_word+1; //get the theoretical position of the first char in next word.
                                //might not always be true if extra spaces, still 
                                //need to deal with this...
    lower_chars(word);   //lowecasercase the word
    rm_punct(word, pulled);  //remove the punctuation
}

//trial version of an improved matching algo
float percentmatched(char original[], char match[]){
    int o_start{0}, m_start{0}; //positions in the arrays
    int o_len{0}, m_len{0}; //length of the strings 
    float match_percent{0}, matches{0}, words{0}; //hold percetage, matches, and words. could cast, but didn't
    char o_word[SIZE], m_word[SIZE]; //arrays to hold the words pulled 
    o_len = strlen(original); 
    m_len = strlen(match);

    while(o_start < o_len){ //improved matching algorithm, now allows for phrases of differing lengths, skipped words, etc

/*      cout << "\nAt indexes: " 
             << o_start << ":\n";
*/
        //pull one word at a time and check them against one another
        extract_word(original, o_start, o_word); //run the extract functions 
        
        while(m_start < m_len){ //nested loop so first extract player 1's word, then search player 2's sentance for it

            //cout << m_start << ' ';

            extract_word(match, m_start, m_word);

/*          cout << "Words were: " << o_word << ":" << m_word 
                 << endl;  //debugging feature but also cool to show the user UPDATE: removed in final version. 
*/            

            if(strcmp(o_word,m_word) == 0){ //if match increment match counter 
                ++matches;
                //cout << "MATCH FOUND" << endl;
            }
        }
        m_start = 0;
        ++ words;
    } 
    cout << endl;
    match_percent = ((matches / words) * 100); //calculate and return percent match
    return match_percent;
}

//remove punctuation from a word
//still need a function to remove excessive space and to check if the phrases are remotely similar
//april 21, right now, p2 can just enter the first word, nothing else and get 100...
void rm_punct(char chars[], char new_word[]){
    int len_chars{0};
    int o{0}, k{0};
    len_chars = strlen(chars);

    for(k = 0, o = 0; k <= len_chars; ++k){
        if(ispunct(chars[k]) == false){
            new_word[o] = chars[k]; //similar proccess to separating words
            ++o;            
            new_word[o+1] = '\0';
        }
    }
}

//convert a character array to lowercase
//easy, just covert everything one by one
void lower_chars(char chars[]){ 
    int len_chars{0}, n{0};
    len_chars = strlen(chars);

    for(n = 0; n < len_chars; ++n){
        chars[n] = tolower(chars[n]);
    }
}

//swap the players names so they are prompted in the game by name
//store names in temporary values then return them to opposite player name arrays
//reinsert end of line character so names of different lengths will swap
void switch_players(char p1[], char p2[]){
    char temp[SIZE], temp2[SIZE];
    int p1_len{0}, p2_len{0}, j{0};
   
    p1_len = strlen(p1);
    p2_len = strlen(p2);
    for(j = 0; j < p1_len; ++j){
        temp[j] = p1[j]; 
        temp[j+1] = '\0';    
    }

    for(j = 0; j < p2_len; ++j){
        temp2[j] = p2[j];
        temp2[j+1] = '\0';
    }

    for(j = 0; j < p2_len; ++j){
        p1[j] = temp2[j];
        p1[j+1] = '\0';
    }

    for(j = 0; j < p1_len; ++j){
        p2[j] = temp[j];
        p2[j+1] = '\0';
    }
}


