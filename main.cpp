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
    double amountWon {};

    int scorePlayer {};
    int scoreDealer {};

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

        std::cout << "\nYour balance is: " << balance << '\n';

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
                    scorePlayer = evaluateHand(playerHand);
                }
                while (scorePlayer < 21 && userChoosesHit());
                break;   

            case 2:                                                 // stand
                scorePlayer = evaluateHand(playerHand);
                break;

            case 3:                                                 // double down
                balance -= amountBet;    
                std::cout << "\nYour balance is now: " << balance << '\n';
                dealCard(playerHand, startingDeck);
                scorePlayer = evaluateHand(playerHand);
                break;

            default:                                                 // anything besides valid choice
                std::cout << "\nThat wasn't an option, please try again. Unless you would like to leave the round of"
                    << "gambling, in which case I'll take the amount that you bet as a tip.\n";
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
        
        if (playerBusted(scorePlayer))
        {
            std::cout << "\nSorry, you busted.\n";
            std::cout << "\nThe casino took your bet of " << amountBet << '\n';
            continue;
        }

        if (dealerBusted(scoreDealer))
        {
            if (blackjack(scorePlayer, playerHand))
            {
                std::cout << "\nCongratulations! The dealer busted and you got Blackjack!\n";
                amountWon = 2.5 * amountBet;
                std::cout << "\nYou won: " << amountWon << '\n';
                balance += amountWon;
            }
            else {
                std::cout << "\nCongratulations, the dealer busted!\n";
                amountWon = 2.0 * amountBet;
                std::cout << "\nYou won: " << amountWon << '\n';
                balance += amountWon;
            }
            continue;
        }

        if (playerWon(scorePlayer, scoreDealer))
        {
            if (blackjack(scorePlayer, playerHand))
            {
                std::cout << "\nCongratulations! You got Blackjack and beat the dealer!\n";
                amountWon = 2.5 * amountBet;
                std::cout << "\nYou won: " << amountWon << '\n';
                balance += amountWon;
            }
            else {
                std::cout << "\nCongratulations, you beat the dealer!\n";
                amountWon = 2.0 * amountBet;
                std::cout << "\nYou won: " << amountWon << '\n';
                balance += amountWon;
            }
        }
        else if (playerTied(scorePlayer, scoreDealer))
        {
            std::cout << "\nYou tied with the dealer.\n";
            std::cout << "\nYou received your bet.\n";
            balance += amountBet;
        }
        else {
            std::cout << "\nSorry, you lost.\n";
            std::cout << "\nThe casino took your bet of " << amountBet << '\n';
        }
    }

    return 0;
}
