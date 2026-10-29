#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

// Dealer requests player to bet money and it returns the bet amount
int requestBet()
{
    std::cout << "\nPlease make your bet: \n";

    int bet;
    std::cin >> bet;
    
    return bet;
}

// deal cards from the startingDeck and remove the drawn cards from the deck
// vec1 would be player hand and vec2 would be deck
void dealCard(std::vector<std::string> &vec1, std::vector<std::string> &vec2)
{
    vec1.push_back(vec2.back());
    std::cout << "\nThe card that you were dealt is: " << vec2.back() << '\n';
    vec2.pop_back();
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
    int score {};
    int numAces {0};                                      

    for (std::size_t i {0}; i < vec1.size(); i++)
    {
        if (vec1.at(i) == "A")
        {
            numAces += 1;
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
        else {
            score += 10;
        } 
    }
    
    switch (numAces)
    {
    case 0:
        break;
    
    case 1:
        if (score <= 10)
        {
            score += 11;
        }
        else
        {
            score += 1;
        }
        break;
    default:
        score += 11 + numAces - 1;
        // if counting one Ace as 11 causes hand to bust, go back and count it as 1 instead
        if (score > 21)
        {
            score -= 10;
        }
        break;
    }
   
    return score;
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

int main()
{
    double balance {500.00};                                        // initial balance of money in USD

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
        playerHand.clear();
        dealerHand.clear();

        int amountBet {requestBet()};                               // run requestBet and set that equal to amountBet so that we can reference the bet amount multiple times
        balance -= amountBet;                                       // subtract the amountBet from the running balance
        std::cout << "\nYour balance is now: " << balance << '\n';
        std::cout << "\nLet's begin with the game.\n";

        dealCard(playerHand, startingDeck);
        dealCard(playerHand, startingDeck);

        dealerReceivesCards(dealerHand, startingDeck);
        std::cout << "\nThe dealer has drawn their first card face down.\n";
        dealerReceivesCards(dealerHand, startingDeck);
        std::cout << "\nThe dealer has drawn their second card face up: " << dealerHand.back() << '\n';

        std::cout << "\nWhat would you like to do now?\n\nYour options are:\n1. Hit\n2. Stand\n3. Double down\n";
        int userChoice {};
        std::cin >> userChoice;

        switch (userChoice)
        {
            case 1:                                                 // hit
                do
                {
                    dealCard(playerHand, startingDeck);
                }
                while ((evaluateHand(playerHand) < 21) && userChoosesHit());
                break;   

            case 2:                                                 // stand
                break;

            case 3:                                                 // double down
                balance -= 2.0 * amountBet;    
                std::cout << "\nYour balance is now: " << balance << '\n';
                dealCard(playerHand, startingDeck);
                break;

            default:                                                 // anything besides valid choice
                std::cout << "\nThat wasn't an option, please try again. Unless you would like to leave the round of"
                    << "gambling, in which case I'll take the amount that you bet as a tip.\n";
                break;
        }

        int scorePlayer {evaluateHand(playerHand)};

        // now that the player has finished making their choice, determine if they busted
        if (scorePlayer > 21)                          // if the player busts
        {
            std::cout << "\nSorry, you busted.\n";
            continue;                                               // start the next round
        }

        // now it's the dealers turn
        std::cout << "\nThe dealer has flipped their first card face up: " << dealerHand.front() << '\n';

        if (evaluateHand(dealerHand) < 17)                          // if the dealer's hand is less than 17
        {
            do
            {
                dealerReceivesCards(dealerHand, startingDeck);      // give the dealer another card
                std::cout << "\nThe dealer has received another card: " << dealerHand.back() << '\n';
            }
            while (evaluateHand(dealerHand) < 17);
        }

        int scoreDealer {evaluateHand(dealerHand)};
        
        if (scoreDealer > 21)                          // if the dealer busts
        {
            std::cout << "\nCongratulations, the dealer has busted.\n";
            balance += 2 * amountBet;
            std::cout << "\nYour balance is now: " << balance << '\n';
        }
        // if it is a tie
        if (scorePlayer == scoreDealer)
        {
            std::cout << "\nYou tied with the dealer.\n";
            balance += amountBet;
            std::cout << "\nYour balance is now: " << balance << '\n';
        }
        else if (scorePlayer > scoreDealer) // if the player wins
        {
            std::cout << "\nCongratulations, you beat the dealer.\n";
            balance += 2 * amountBet;
            std::cout << "\nYour balance is now: " << balance << '\n';
        }
        else if (scorePlayer < scoreDealer)   // if the player loses
        {
            std::cout << "\nSorry, you lost the round\n";
        }
    }

    return 0;
}
