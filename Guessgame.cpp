#include <iostream>
#include <string>
#include <string.h>
#include <math.h>
#include <time.h> 

using namespace std;

int main()
{
	int input,guess,trying=1;

	cout << "NOTE You Have 3 Trys Only\n" ;
	srand(time(NULL));
	guess = rand() % 100;
	for (; trying <= 3; trying++)
	{
		if (trying == 1)
		{
			system("color 0A");
			cout <<"guess the number from 1 to 100 "<<"["<< " TRY : " << trying <<"]"<< endl;
			cin >> input;
	
			if (input < guess)
			{
				cout << "the secret number is higher" << endl;
			}
			else if (input > guess)
			{
				cout << "the secret number is lower" << endl;
			}
			else if (input == guess)
			{
				system("cls");
				cout << "wOow you win!! ??" << endl;
				break;
			}
		}
		else if (trying == 2)
		{
			system("color 0B");
			cout << "guess the number from 1 to 100 " << "[" << " TRY : " << trying << "]" << endl;
			cin >> input;

			if (input < guess)
			{
				cout << "the secret number is higher" << endl;
			}
			else if (input > guess)
			{
				cout << "the secret number is lower" << endl;
			}
			else if (input == guess)
			{
				system("cls");
				cout << "wOow you win!! ??" << endl;
				break;
			}
		}
		else if (trying == 3)
		{
			system("color 0C");
			cout << "guess the number from 1 to 100 " << "[" << " TRY : " << trying << "]" << endl;
			cin >> input;

			if (input < guess)
			{
				cout << "the secret number is higher" << endl;
			}
			else if (input > guess)
			{
				cout << "the secret number is lower" << endl;
			}
			else if (input == guess)
			{
				system("cls");
				cout << "wOow you win!! ??" << endl;
				break;
			}
		}
		else
			break;
	}
	system("pause");
	return 0;
}