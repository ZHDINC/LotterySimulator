#include<iostream>
#include<random>
#include<vector>
#include<algorithm>
#include<Windows.h>

void PickNormalNumbers(std::mt19937& gen, std::uniform_int_distribution<>& normalNumbers, std::vector<int>& pickNumbers, std::uniform_int_distribution<>& powerball, int& powerballPick)
{
    for (int i = 0; i < 5; i++)
    {
        if (i == 0)
        {
            pickNumbers.push_back(normalNumbers(gen));
            continue;
        }
        bool notPicked = true;
        bool match = false;
        while (notPicked)
        {
            int nextPick = normalNumbers(gen);
            for (int x : pickNumbers)
            {
                if (x == nextPick)
                {
                    match = true;
                    break;
                }
            }
            if (!match)
            {
                notPicked = false;
                pickNumbers.push_back(nextPick);
            }
            match = false;
        }
    }
    powerballPick = powerball(gen);
    std::sort(pickNumbers.begin(), pickNumbers.end());
}

void PrintNumbers(HANDLE hConsole, const std::vector<int>& pickNumbers, const int powerballPick)
{
    
    for (int number : pickNumbers)
    {
        std::cout << number << " ";
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    std::cout << powerballPick << '\n';
    SetConsoleTextAttribute(hConsole, 7);
}

struct Winnings
{
    int pbNoNumbers = 0;
    int pbOneNumber = 0;
    int pbTwoNumbers = 0;
    int pbThreeNumbers = 0;
    int pbFourNumbers = 0;
    int pbFiveNumbers = 0;
    int ThreeNumbers = 0;
    int FourNumbers = 0;
    int FiveNumbers = 0;
    int SpendingAndWinnings = 0;

    void PrintWinnings()
    {
        std::cout << "Powerball itself " << pbNoNumbers << " times. Winnings: $" << pbNoNumbers * 4 << '\n';
        std::cout << "Powerball plus one number " << pbOneNumber << " times. Winnings: $" << pbOneNumber * 4 << '\n';
        std::cout << "Powerball plus two numbers " << pbTwoNumbers << " times. Winnings: $" << pbTwoNumbers * 7 << '\n';
        std::cout << "Powerball plus three numbers " << pbThreeNumbers << " times. Winnings: $" << pbThreeNumbers * 100 << '\n';
        std::cout << "Powerball plus four numbers " << pbFourNumbers << " times. Winnings: $" << pbFourNumbers * 50000 << '\n';
        std::cout << "Powerball plus five numbers " << pbFiveNumbers << " time. Winnings: $" << 121000000 << " (supposing no other winners)\n";
        std::cout << "Three numbers " << ThreeNumbers << " times. Winnings: $" << ThreeNumbers * 7 << '\n';
        std::cout << "Four numbers " << FourNumbers << " times. Winnings: $" << FourNumbers * 100 << '\n';
        std::cout << "Five numbers " << FiveNumbers << " times. Winnings: $" << FiveNumbers * 1000000 << '\n';
    }

    int TotalWinnings()
    {
        return pbNoNumbers * 4 + pbOneNumber * 4 + pbTwoNumbers * 7 + pbThreeNumbers * 100 + pbFourNumbers * 50000 + 121000000 + ThreeNumbers * 7 + FourNumbers * 100 + FiveNumbers * 1000000;
    }
};

bool AttemptWin(const std::vector<int>& actualNumbers, const std::vector<int>& pickNumbers, const int actualPowerBall, const int powerballPick, Winnings& winnings)
{
    bool allNumbers = true;
    bool gotPowerBall = false;
    int pickVectorIndex = 0;
    int numberMatch = 0;
    for (auto number : actualNumbers)
    {
        if (number == pickNumbers[pickVectorIndex])
        {
            numberMatch++;
        }
        else
        {
            allNumbers = false;
        }
        pickVectorIndex++;
    }
    if (actualPowerBall == powerballPick)
    {
        gotPowerBall = true;
    }
    else
    {
        allNumbers = false;
    }
    // Determining winnings
    if (gotPowerBall)
    {
        if (numberMatch == 0)
        {
            winnings.pbNoNumbers++;
            winnings.SpendingAndWinnings += 4;
        }
        if (numberMatch == 1)
        {
            winnings.pbOneNumber++;
            winnings.SpendingAndWinnings += 4;
        }
        if (numberMatch == 2)
        {
            winnings.pbTwoNumbers++;
            winnings.SpendingAndWinnings += 7;
        }
        if (numberMatch == 3)
        {
            winnings.pbThreeNumbers++;
            winnings.SpendingAndWinnings += 100;
        }
        if (numberMatch == 4)
        {
            winnings.pbFourNumbers++;
            winnings.SpendingAndWinnings += 50000;
        }
        if (numberMatch == 5)
        {
            winnings.pbFiveNumbers++;
            winnings.SpendingAndWinnings += 121000000;
        }
    }
    else
    {
        if (numberMatch == 3)
        {
            winnings.ThreeNumbers++;
            winnings.SpendingAndWinnings += 7;
        }
        if (numberMatch == 4)
        {
            winnings.FourNumbers++;
            winnings.SpendingAndWinnings += 100;
        }
        if (numberMatch == 5)
        {
            winnings.FiveNumbers++;
            winnings.SpendingAndWinnings += 1000000;
        }
    }
    return allNumbers;
}

int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> normalNumbers(1, 69);
    std::uniform_int_distribution<> powerball(1, 26);

    std::vector<int> pickNumbers;
    int powerballPick;
    std::vector<int> actualNumbers{ 14, 34, 37, 55, 63};
    int actualPowerBall = 20;
    std::cout << "Desired run\n1) Pick single set of numbers\n2) Pick multiple sets of numbers\n3) Run until winning numbers picked\n";
    int kindOfRun;
    std::cin >> kindOfRun;
    switch (kindOfRun)
    {
    case 1:
        PickNormalNumbers(gen, normalNumbers, pickNumbers, powerball, powerballPick);
        PrintNumbers(hConsole, pickNumbers, powerballPick);
        break;
    case 2:
    {
        std::cout << "Number of runs?\n";
        int numberOfRuns;
        std::cin >> numberOfRuns;
        for (int i = 0; i < numberOfRuns; i++)
        {
            PickNormalNumbers(gen, normalNumbers, pickNumbers, powerball, powerballPick);
            PrintNumbers(hConsole, pickNumbers, powerballPick);
            pickNumbers.clear();
        }
        break;
    }
    case 3:
    {
        bool whileUnsuccessful = true;
        int attempts = 0;
        int numberOfOverflows = 0;
        std::cout << "Do you want to print out your win/loss statistics as it runs? (Y/N) ";
        bool printWinLoss = false;
        char winLossStatistics;
        std::cin >> winLossStatistics;
        int printWinLossFrequency = 100;
        if (toupper(winLossStatistics) == 'Y')
        {
            printWinLoss = true;
            std::cout << "After how many attempts do you wish to see your win/loss statistics?\nCAUTION: More frequent printing to the console will cause the program to run slower.\n";
            std::cin >> printWinLossFrequency;
        }
        Winnings winnings;
        while (whileUnsuccessful)
        {
            attempts++;
            winnings.SpendingAndWinnings -= 2;
            if (attempts == INT_MIN)
            {
                numberOfOverflows++;
            }
            PickNormalNumbers(gen, normalNumbers, pickNumbers, powerball, powerballPick);
            //PrintNumbers(hConsole, pickNumbers, powerballPick);
            if (AttemptWin(actualNumbers, pickNumbers, actualPowerBall, powerballPick, winnings))
            {
                whileUnsuccessful = false;
                std::cout << "Total victory after " << attempts << " attempts (overflowed " << numberOfOverflows << " times).\n";
                winnings.PrintWinnings();
                int totalWinnings = winnings.TotalWinnings();
                std::cout << "You've spent $" << attempts * 2 << " and won $" << totalWinnings << ".\n";
            }
            if (printWinLoss && attempts % printWinLossFrequency == 0)
            {
                std::cout << "You are currently " << winnings.SpendingAndWinnings << '\n';
            }
            pickNumbers.clear();
        }
    }
    default:
        break;
    }

    std::cout << "Press any key to continue...\n";
    char key;
    std::cin >> key;
    
    CloseHandle(hConsole);
}