#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

// Dealer requests player to bet money and it returns the bet amount
double requestBet(double balance)
{
    double bet;
    double balance2{balance}; // balance2 is created so we can alter its value and restore it to starting value of balance if needed

    // keep asking for input until it is a correct input
    do
    {
        std::cout << "\nPlease make your bet: \n";
        balance2 = balance; // this is where we restore balance2 to the starting value of balance

        while (!(std::cin >> bet)) // while the input is not a valid double
        {
            std::cout << "\nThat was not a valid input. Try again.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        balance2 -= bet;  // this will be used to check if the bet amount is greater than funds available
    } while (balance2 < 0.0); // ask for input again if resulting balance is negative
    
    return bet; // return the amount that the user bet
}

// deal cards from the startingDeck and remove the drawn cards from the deck
// vec1 would be player hand and vec2 would be deck
void dealCard(std::vector<std::string> &vec1, std::vector<std::string> &vec2)
{
    vec1.push_back(vec2.back()); // player receives card from the back of the deck
    std::cout << "\nThe card that you were dealt is: " << vec2.back() << '\n';
    vec2.pop_back(); // remove the card (that was dealt out) from the deck
}

// deal cards from deck to the dealer and remove the drawn cards from the deck
// vec3 would be dealer's hand and vec2 would be the deck
// since the dealer has one card revealed and the other face down, the text output of what
//      the cards are will have to be taken care of in the main function
void dealerReceivesCards(std::vector<std::string> &vec3, std::vector<std::string> &vec2)
{
    vec3.push_back(vec2.back());
    vec2.pop_back();
}

// determines the numerical value of the hand with Ace being counted as 1 point
int evaluateHand(std::vector<std::string> &vec1)
{
    int score {};       // this will be used to track the numerical score of the hand
    int numAces {0};    // this will be used to track the number of Aces in the hand                                      

    for (std::size_t i {0}; i < vec1.size(); i++)   // for each card in hand
    {
        if (vec1.at(i) == "A")
        {
            numAces += 1;   // increment the Aces tracker by 1
        }
        else if (vec1.at(i) == "2")
        {
            score += 2;
        }
        else if (vec1.at(i) == "3")
        {
            score += 3;
        }
        else if (vec1.at(i) == "4")
        {
            score += 4;
        }
        else if (vec1.at(i) == "5")
        {
            score += 5;
        }
        else if (vec1.at(i) == "6")
        {
            score += 6;
        }
        else if (vec1.at(i) == "7")
        {
            score += 7;
        }
        else if (vec1.at(i) == "8")
        {
            score += 8;
        }
        else if (vec1.at(i) == "9")
        {
            score += 9;
        }
        else {          // if the card is 10, J, Q, or K
            score += 10;
        } 
    }
    
    switch (numAces)                // this is where we handle the score of hand depending on number of Aces
    {
    case 0:                         // if no Aces, then the switch logic is not neeted
        break;
    
    case 1:                         // if only one Ace in hand
        if (score <= 10)
        {
            score += 11;            // if the score is small enough that it can handle 11 more points, go with that
        }
        else
        {
            score += 1;             // otherwise, only add 1 point to avoid busting
        }
        break;
    default:                        // this will handle any amount of Aces greater than 1
        score += 11 + numAces - 1;  // count one Aces as 11 points and the rest of the Aces as 1 to be able to get highest score
        
        if (score > 21)             // if counting one Ace as 11 causes hand to bust, go back and count it as 1 instead
        {
            score -= 10;            // subtracting 10 from score results in the Ace being counted as 1, since 11 - 10 = 1
        }
        break;
    }
   
    return score;                   // return the score of the hand
}

// return true if hit is chosen
bool userChoosesHit()
{
    std::cout << "\nWhat would you like to do now?\n\nYour options are:\n1. Hit\n2. Stand\n";
    int userChoice {};
    std::cin >> userChoice;

    if (userChoice == 1)
    {
        return true;
    } else {
        return false;
    }
}

// return true if the player hand busted
bool playerBusted(int playerScore)
{
    if (playerScore > 21)
    {
        return true;
    }
    else {
        return false;
    }
}

// return true if dealer hand busted
bool dealerBusted(int dealerScore)
{
    if (dealerScore > 21)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// return true if the player won
bool playerWon (int playerScore, int dealerScore)
{
    if (playerScore > dealerScore)
    {
        return true;
    }
    else {
        return false;
    }
}

// return true if the player and dealer tied
bool playerTied (int playerScore, int dealerScore)
{
    if (playerScore == dealerScore)
    {
        return true;
    }
    else {
        return false;
    }
}

// return true if teh player got Blackjack
bool blackjack (int playerScore, std::vector<std::string> &vec1)
{
    if (playerScore == 21 && vec1.size() == 2)
    {
        return true;
    }
    else {
        return false;
    }
}

int main()
{
    double balance {500.00};                                       // initial balance of money in USD
    double amountWon {};                                           // this will be used to display money won from round

    int scorePlayer {};                                            // score of player's hand
    int scoreDealer {};                                            // score of dealer's hand

    // normal 52-card deck, with each card treated as a string
    std::vector<std::string> startingDeck {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K",
        "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K",
        "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K",
        "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

    std::vector<std::string> playerHand {};                         // initialize player hand with zero entries
    std::vector<std::string> dealerHand {};                         // initialize dealer hand with zero entries

    // shuffle deck. rd is fed into rng so that everytime the program is run, the shuffle is random
    auto rd = std::random_device{};                       
    auto rng = std::default_random_engine{rd()};
    std::shuffle(std::begin(startingDeck), std::end(startingDeck), rng);
    std::cout << "\nThe game of Blackjack has begun.\n";

    while (balance > 0.00)                                          // only let the game continue while the player has some amount of money
    {
        playerHand.clear();                                         // empty the player's hand
        dealerHand.clear();                                         // empty the dealer's hand

        std::cout << "\nYour balance is: " << balance << '\n';

        double amountBet {requestBet(balance)};                     // run requestBet and set that equal to amountBet so that we can reference the bet amount multiple times
        balance -= amountBet;                                       // subtract the amountBet from the running balance

        std::cout << "\nYour balance is now: " << balance << '\n';
        std::cout << "\nLet's begin with the game.\n";

        // deal out two cards from shuffled deck to player
        dealCard(playerHand, startingDeck);
        dealCard(playerHand, startingDeck);

        // deal out two cards to dealer from deck
        dealerReceivesCards(dealerHand, startingDeck);
        std::cout << "\nThe dealer has drawn their first card face down.\n";
        dealerReceivesCards(dealerHand, startingDeck);
        std::cout << "\nThe dealer has drawn their second card face up: " << dealerHand.back() << '\n';

        // offer choice of hit, stand, or double down to player
        std::cout << "\nWhat would you like to do now?\n\nYour options are:\n1. Hit\n2. Stand\n3. Double down\n";
        int userChoice {};
        
        while (!(std::cin >> userChoice) || (userChoice > 3) || ((balance <= amountBet) && (userChoice == 3))) // if the user inputs anything besides int or greather than 3
        {
            std::cout << "\nThat input was incorrect. Please try again.\n";
            std::cin.clear(); // reset input errors
            std::cin.ignore(10000, '\n'); // remove bad input
        }

        switch (userChoice)
        {
            // keep dealing another card to player while they keep opting for hit and the score of their hand is less than 21
            case 1:                                                 // hit
                do
                {
                    dealCard(playerHand, startingDeck);             // deal an additional card to player
                    scorePlayer = evaluateHand(playerHand);         // reevaluate the score of player's hand
                }
                while (scorePlayer < 21 && userChoosesHit());
                break;   
            
            // if player chooses to stand, evaluate the score of their hand and move on
            case 2:                                                 // stand
                scorePlayer = evaluateHand(playerHand);
                break;

            // if player chooses to double down, remove additional money equal to their initial bet and deal one more card to them
            case 3:                                                 // double down
                balance -= amountBet;    
                std::cout << "\nYour balance is now: " << balance << '\n';
                dealCard(playerHand, startingDeck);
                scorePlayer = evaluateHand(playerHand);             // reevaluate the score of player's hand
                break;

            default:                                                 // anything besides valid choice
                std::cout << "\nThat wasn't an option, please try again. Unless you would like to leave the round of "
                    << "gambling,\nin which case I'll take the amount that you bet as a tip.\n";
                break;
        }

        // now it's the dealers turn
        std::cout << "\nThe dealer has flipped their first card face up: " << dealerHand.front() << '\n';

        scoreDealer = evaluateHand(dealerHand);

        if (scoreDealer < 17)                          // if the dealer's hand is less than 17
        {
            do
            {
                dealerReceivesCards(dealerHand, startingDeck);      // give the dealer another card
                std::cout << "\nThe dealer has received another card: " << dealerHand.back() << '\n';
                scoreDealer = evaluateHand(dealerHand);
            }
            while (scoreDealer < 17);
        }
        
        // if the player busted, move on to the next round
        if (playerBusted(scorePlayer))
        {
            std::cout << "\nSorry, you busted.\n";
            std::cout << "\nThe casino took your bet of " << amountBet << '\n';
            continue;                                                               // move on to the next round
        }

        // if the dealer busts, we still want to reward the player if they got blackjack
        if (dealerBusted(scoreDealer))
        {
            if (blackjack(scorePlayer, playerHand))                                             // if player got blackjack
            {
                std::cout << "\nCongratulations! The dealer busted and you got Blackjack!\n";
                amountWon = 2.5 * amountBet;                                                    // this rewards player at 3:2 ratio
                std::cout << "\nYou won: " << amountWon << '\n';
                balance += amountWon;
            }
            else {                                                                              // if player did not get blackjack
                std::cout << "\nCongratulations, the dealer busted!\n";
                amountWon = 2.0 * amountBet;                                                    // this rewards player at normal 1:1 ratio
                std::cout << "\nYou won: " << amountWon << '\n';
                balance += amountWon;
            }
            continue;                                                                           // if the dealer busted, then move onto next round
        }

        if (playerWon(scorePlayer, scoreDealer))                                                // if the player beat the dealer
        {
            if (blackjack(scorePlayer, playerHand))                                             // if player got blackjack
            {
                std::cout << "\nCongratulations! You got Blackjack and beat the dealer!\n";
                amountWon = 2.5 * amountBet;
                std::cout << "\nYou won: " << amountWon << '\n';
                balance += amountWon;
            }
            else {                                                                              // if player did not get blackjack
                std::cout << "\nCongratulations, you beat the dealer!\n";
                amountWon = 2.0 * amountBet;
                std::cout << "\nYou won: " << amountWon << '\n';
                balance += amountWon;
            }
        }
        else if (playerTied(scorePlayer, scoreDealer))                                          // if player and dealer tied
        {
            std::cout << "\nYou tied with the dealer.\n";
            std::cout << "\nYou received your bet.\n";
            balance += amountBet;
        }
        else {                                                                                  // if player lost
            std::cout << "\nSorry, you lost.\n";
            std::cout << "\nThe casino took your bet of " << amountBet << '\n';
        }
    }

    return 0;
}
