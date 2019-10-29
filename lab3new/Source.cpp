#define _CRT_SECURE_NO_WARNINGS //delete this if dont use visual studio for compile (sprintf dont safe)
//Fl libs
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <Fl/Fl_Light_Button.H>
#include <FL/Fl_Output.H>
//c libs
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <stdio.h>

using namespace std;

Fl_Light_Button* btnA[16];//operand A buttons
Fl_Light_Button* btnB[16];//operand B buttons
Fl_Light_Button* btn_res[16];//result buttons

int vOp = 0;//flag for choose operations
int vSys = 0;//flag for choose system notation

int stringA[16];//stores data about numberA in binary number system
int stringB[16];//stores data about numberB in binary number system
int stringC[16];//stores data about number_res in binary number system

Fl_Input* inputA;//input operand A
Fl_Input* inputB;//input operand B
Fl_Input* inputC;//input operand C

Fl_Button* btnsys;//button with operations

Fl_Box* aBox[16];//label operand A
Fl_Box* bBox[16];//label operand B
Fl_Box* cBox[16];//label operand C

Fl_Box* more;//use when overflow stack nums

const char* name[] = { "+", "-", "*", "/", "<<", ">>", "&", "|", "~" };//operations initialization list
const char* labels[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15" };//number digit initialization list
const char* namesys[] = { "DEC", "OCT", "HEX" };//operations initialization list

void output()
{
	int decA = 0; int decB = 0; int decC = 0; int stepen = 0;
	for (int l = 0; l < 16; l++)
	{
		stringA[l] = btnA[l]->label()[0];
		stringB[l] = btnB[l]->label()[0];
		stringC[l] = btn_res[l]->label()[0];
		if (stringA[l] == '1')
		{
			decA += pow(2, stepen);
		}
		if (stringB[l] == '1')
		{
			decB += pow(2, stepen);
		}
		if (stringC[l] == '1')
		{
			decC += pow(2, stepen);
		}
		stepen++;
	}
	char str1[16]; char str2[16]; char str3[16];
	switch (vSys)
	{
	case 0:
		sprintf(str1, "%d", decA);//%o-oct, %d-dec, %X-hex
		sprintf(str2, "%d", decB);//%o-oct, %d-dec, %X-hex
		sprintf(str3, "%d", decC);//%o-oct, %d-dec, %X-hex
		break;
	case 1:
		sprintf(str1, "%o", decA);//%o-oct, %d-dec, %X-hex
		sprintf(str2, "%o", decB);//%o-oct, %d-dec, %X-hex
		sprintf(str3, "%o", decC);//%o-oct, %d-dec, %X-hex
		break;
	case 2:
		sprintf(str1, "%X", decA);//%o-oct, %d-dec, %X-hex
		sprintf(str2, "%X", decB);//%o-oct, %d-dec, %X-hex
		sprintf(str3, "%X", decC);//%o-oct, %d-dec, %X-hex
		break;
	}
	inputA->value(str1);
	inputB->value(str2);
	inputC->value(str3);
}

char digit(char c)
{
	switch (c)
	{
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	case 'A': return 10;
	case 'B': return 11;
	case 'C': return 12;
	case 'D': return 13;
	case 'E': return 14;
	case 'F': return 15;
	default:
		return 0;
	}
}

void inputsysa(Fl_Widget* o, void* obj)
{
	more->label("Stable");
	more->labelcolor(FL_GREEN);
	inputA->textcolor(FL_BLACK);
	int test1 = 0;
	int test2[128];
	int test3 = 0;
	const char* temp;
	int len;

	char str3[16];//output
	int i = 0;
	Fl_Input* input = (Fl_Input*)obj;

	switch (vSys)
	{
	case 0:
		for (int j = 0; j < 16; j++)//Clear
		{
			btnA[j]->label("0");
			btnA[j]->value(0);
		}
		test1 = atoi(input->value());//input
		for (i = 0; test1; i++)//dec >> bin
		{
			test2[i] = test1 % 2;
			test1 /= 2;
		}
		if (vOp == 8)
		{
			for (i--; i >= 0; i--)
			{
				if (test2[i] == 0) { btnA[i]->label("1"); btnA[i]->value(1); }
				else if (test2[i] == 1) { btnA[i]->label("0"); btnA[i]->value(0); }
			}
		}
		else
		{
			for (i--; i >= 0; i--)
			{
				if (test2[i] == 0) { btnA[i]->label("0"); btnA[i]->value(0); }
				else if (test2[i] == 1) { btnA[i]->label("1"); btnA[i]->value(1); }
			}
		}
		break;
	case 1:
		for (int j = 0; j < 16; j++)//Clear
		{
			btnA[j]->label("0");
			btnA[j]->value(0);
		}
		test1 = atoi(inputA->value());//input
		for (i = 0; test1; i++)//check num of positions
		{
			test2[i] = test1 % 10;
			if (test2[i] > 7) { inputA->value("Wrong!"); more->label("Warning!"); more->labelcolor(FL_RED); inputA->textcolor(FL_RED); return; }
			test1 /= 10;
		}
		for (i--; i >= 0; i--)//oct >> dec
		{
			test3 += test2[i] * pow(8, i);
		}
		for (i = 0; test3; i++)//dec >> bin
		{
			test2[i] = test3 % 2;
			test3 /= 2;
		}
		if (vOp == 8)
		{
			for (i--; i >= 0; i--)
			{
				if (test2[i] == 0) { btnA[i]->label("1"); btnA[i]->value(1); }
				else if (test2[i] == 1) { btnA[i]->label("0"); btnA[i]->value(0); }
			}
		}
		else
		{
			for (i--; i >= 0; i--)
			{
				if (test2[i] == 0) { btnA[i]->label("0"); btnA[i]->value(0); }
				else if (test2[i] == 1) { btnA[i]->label("1"); btnA[i]->value(1); }
			}
		}
		break;
	case 2:
		for (int j = 0; j < 16; j++)//Clear
		{
			btnA[j]->label("0");
			btnA[j]->value(0);
		}
		temp = inputA->value();//input
		len = strlen(temp);//check num of positions
		len--;
		for (int i = 0; len >= 0; len--, i++)//hex >> dec
		{
			test3 += digit(temp[len]) * pow(16, i);
		}
		for (i = 0; test3; i++)//dec >> bin
		{
			test2[i] = test3 % 2;
			test3 /= 2;
		}
		if (vOp == 8)
		{
			for (i--; i >= 0; i--)
			{
				if (test2[i] == 0) { btnA[i]->label("1"); btnA[i]->value(1); }
				else if (test2[i] == 1) { btnA[i]->label("0"); btnA[i]->value(0); }
			}
		}
		else
		{
			for (i--; i >= 0; i--)
			{
				if (test2[i] == 0) { btnA[i]->label("0"); btnA[i]->value(0); }
				else if (test2[i] == 1) { btnA[i]->label("1"); btnA[i]->value(1); }
			}
		}
		break;
	}
}

void inputsysb(Fl_Widget* o, void* obj)
{
	more->label("Stable");
	more->labelcolor(FL_GREEN);
	inputB->textcolor(FL_BLACK);
	int test1 = 0;
	int test2[128];
	int test3 = 0;
	const char* temp;
	int len;

	char str3[16];//output
	int i = 0;
	Fl_Input* input = (Fl_Input*)obj;

	switch (vSys)
	{
	case 0:
		for (int j = 0; j < 16; j++)//Clear
		{
			btnB[j]->label("0");
			btnB[j]->value(0);
		}
		test1 = atoi(inputB->value());//input
		for (i = 0; test1; i++)//dec >> bin
		{
			test2[i] = test1 % 2;
			test1 /= 2;
		}
		if (vOp == 8)
		{
			for (i--; i >= 0; i--)
			{
				if (test2[i] == 0) { btnB[i]->label("1"); btnB[i]->value(1); }
				else if (test2[i] == 1) { btnB[i]->label("0"); btnB[i]->value(0); }
			}
		}
		else
		{
			for (i--; i >= 0; i--)
			{
				if (test2[i] == 0) { btnB[i]->label("0"); btnB[i]->value(0); }
				else if (test2[i] == 1) { btnB[i]->label("1"); btnB[i]->value(1); }
			}
		}
		break;
	case 1:
		for (int j = 0; j < 16; j++)//Clear
		{
			btnB[j]->label("0");
			btnB[j]->value(0);
		}
		test1 = atoi(inputB->value());//input
		for (i = 0; test1; i++)//check num of positions
		{
			test2[i] = test1 % 10;
			if (test2[i] > 7) { inputB->value("Wrong!"); more->label("Warning!"); more->labelcolor(FL_RED); inputB->textcolor(FL_RED); return; }
			test1 /= 10;
		}
		for (i--; i >= 0; i--)//oct >> dec
		{
			test3 += test2[i] * pow(8, i);
		}
		for (i = 0; test3; i++)//dec >> bin
		{
			test2[i] = test3 % 2;
			test3 /= 2;
		}
		if (vOp == 8)
		{
			for (i--; i >= 0; i--)
			{
				if (test2[i] == 0) { btnB[i]->label("1"); btnB[i]->value(1); }
				else if (test2[i] == 1) { btnB[i]->label("0"); btnB[i]->value(0); }
			}
		}
		else
		{
			for (i--; i >= 0; i--)
			{
				if (test2[i] == 0) { btnB[i]->label("0"); btnB[i]->value(0); }
				else if (test2[i] == 1) { btnB[i]->label("1"); btnB[i]->value(1); }
			}
		}
		break;
	case 2:
		for (int j = 0; j < 16; j++)//Clear
		{
			btnB[j]->label("0");
			btnB[j]->value(0);
		}
		temp = inputB->value();//input
		len = strlen(temp);//check num of positions
		len--;
		for (int i = 0; len >= 0; len--, i++)//hex >> dec
		{
			test3 += digit(temp[len]) * pow(16, i);
		}
		for (i = 0; test3; i++)//dec >> bin
		{
			test2[i] = test3 % 2;
			test3 /= 2;
		}
		if (vOp == 8)
		{
			for (i--; i >= 0; i--)
			{
				if (test2[i] == 0) { btnB[i]->label("1"); btnB[i]->value(1); }
				else if (test2[i] == 1) { btnB[i]->label("0"); btnB[i]->value(0); }
			}
		}
		else
		{
			for (i--; i >= 0; i--)
			{
				if (test2[i] == 0) { btnB[i]->label("0"); btnB[i]->value(0); }
				else if (test2[i] == 1) { btnB[i]->label("1"); btnB[i]->value(1); }
			}
		}
		break;
	}
}

void results(Fl_Widget* o, void* obj)
{
	Fl_Light_Button* button = (Fl_Light_Button*)obj;
	if (button->label() == "0") { button->value(0); }
	else if (button->label() == "1") { button->value(1); }
}

void systems(Fl_Widget* o)
{
	vSys++;
	if (vSys > 2) { vSys = 0; }
	o->label(namesys[vSys]);
	output();
}

void Calc(Fl_Widget* o, void* obj)
{
	inputA->textcolor(FL_BLACK);
	more->labelcolor(FL_GREEN);
	inputC->textcolor(FL_BLACK);
	inputC->textcolor(FL_BLACK);
	more->label("Stable");

	Fl_Light_Button** button = (Fl_Light_Button**)obj;
	for (int j = 0; j < 16; j++)//Clear
	{
		btn_res[j]->label("0");
		button[j]->value(0);
		btn_res[j]->activate();
	}
	char str3[16];//output

	int test1[128];

	int decA = 0; int decB = 0; int stepen = 0; int rasn = 0;
	for (int l = 0; l < 16; l++)
	{
		stringA[l] = btnA[l]->label()[0];
		stringB[l] = btnB[l]->label()[0];
		if (stringA[l] == '1')
		{
			decA += pow(2, stepen);
		}
		if (stringB[l] == '1')
		{
			decB += pow(2, stepen);
		}
		stepen++;
	}
	switch (vOp)
	{
	case 0:
		rasn = decA + decB;
		break;
	case 1:
		rasn = decA - decB;
		break;
	case 2:
		rasn = decA * decB;
		break;
	case 3:
		if (decB == 0) { inputC->textcolor(FL_RED); inputC->value("Div by zero"); return; }
		rasn = decA / decB;
		break;
	case 4:
		rasn = decA << decB;
		break;
	case 5:
		rasn = decA >> decB;
		break;
	case 6:
		rasn = decA & decB;
		break;
	case 7:
		rasn = decA | decB;
		break;
	case 8:
		rasn = decA;
		break;
	}
	int answer = rasn;
	//
	if (vOp != 8)
	{
		switch (vSys)
		{
		case 0:
			sprintf(str3, "%d", answer);//%o-oct, %d-dec, %X-hex
			break;
		case 1:
			sprintf(str3, "%o", answer);//%o-oct, %d-dec, %X-hex
			break;
		case 2:
			sprintf(str3, "%X", answer);//%o-oct, %d-dec, %X-hex
			break;
		}
		inputC->value(str3);
	}
	//
	if (answer > 65535 || answer < -65535) 
	{ 
		more->labelcolor(FL_RED); 
		more->label("Overflowed"); 
		for (int i = 0; i < 16; i++)
		{
			btn_res[i]->deactivate();
		}
		inputC->textcolor(FL_RED);
		return; 
	}
	int i;
	for (i = 0; rasn; i++)
	{
		test1[i] = rasn % 2;
		rasn /= 2;
	}
	if (vOp == 8)
	{
		for (i--; i >= 0; i--)
		{
			if (test1[i] == 0) { btn_res[i]->label("1"); button[i]->value(1); }
			else if (test1[i] == 1) { btn_res[i]->label("0"); button[i]->value(0); }
		}
		//
		int decC = 0; stepen = 0;
		for (int l = 0; l < 16; l++)
		{
			stringC[l] = btn_res[l]->label()[0];
			if (stringC[l] == '1')
			{
				decC += pow(2, stepen);
			}
			stepen++;
		}
		switch (vSys)
		{
		case 0:
			sprintf(str3, "%d", decC);//%o-oct, %d-dec, %X-hex
			break;
		case 1:
			sprintf(str3, "%o", decC);//%o-oct, %d-dec, %X-hex
			break;
		case 2:
			sprintf(str3, "%X", decC);//%o-oct, %d-dec, %X-hex
			break;
		}
		inputC->value(str3);
	}
	else
	{
		for (i--; i >= 0; i--)
		{
			if (test1[i] == 0) { btn_res[i]->label("0"); button[i]->value(0); }
			else if (test1[i] == 1) { btn_res[i]->label("1"); button[i]->value(1); }
		}
	}
}

void opfunc(Fl_Widget* o)
{
	vOp++;
	if (vOp > 8) 
	{ 
		vOp = 0; 
		for (int i = 0; i < 16; i++)
		{
			btnB[i]->activate();
		}
		inputB->activate();
	}
	else if (vOp == 8) 
	{ 
		for (int i = 0; i < 16; i++)
		{
			btnB[i]->deactivate();
		}
		inputB->deactivate();
	}
	o->label(name[vOp]);
}

void test1(Fl_Widget* o)
{
	inputA->textcolor(FL_BLACK);
	inputB->textcolor(FL_BLACK);
	inputC->textcolor(FL_BLACK);
	more->label("Stable");
	more->labelcolor(FL_GREEN);
	if (o->label() == "0")
	{
		o->label("1");
	}
	else { o->label("0"); }
	output();
}

int main(int argc, char** argv)
{
	Fl_Window* window = new Fl_Window(269, 222, 859, 262, "Lab3 by Yaroslav Ilyushin");

	Fl_Button btnop{ 5, 65, 64, 20 };
	btnop.label(name[vOp]);
	btnop.box(_FL_RFLAT_BOX);
	btnop.callback(opfunc);

	Fl_Button btnres{ 5, 150, 64, 20 };
	btnres.label("=");
	btnres.box(_FL_RFLAT_BOX);

	Fl_Box lA{ 0,25,20,16 }; Fl_Box lB{ 0,110,20,16 }; Fl_Box lC{ 0,190,20,16 };
	lA.label("A"); lB.label("B"); lC.label("C");

	Fl_Box clk{ 670, 57,5,143 };
	clk.label("B I N A R Y C A L C");
	clk.labelsize(19);
	clk.align(128);

	inputA = new Fl_Input{ 720, 25, 75, 24 }; inputA->callback(inputsysa, inputA); inputA->value("0");

	inputB = new Fl_Input{ 720, 110, 75, 24 }; inputB->callback(inputsysb, inputB); inputB->value("0");

	inputC = new Fl_Input{ 720, 190, 75, 24 }; inputC->value("0");

	btnsys = new Fl_Button{ 725,230,64,20 };
	btnsys->label(namesys[vSys]);
	btnsys->box(_FL_RFLAT_BOX);
	btnsys->callback(systems);

	more = new Fl_Box{ 5,234,80,16 };
	more->label("Stable");
	more->labelcolor(FL_GREEN);

	for (int i = 0, x = 560; i < 16; i++)
	{
		btnA[i] = new Fl_Light_Button{ x, 25, 25, 20 };
		btnA[i]->label("0");
		btnA[i]->color2(FL_RED);
		btnA[i]->callback(test1);

		btnB[i] = new Fl_Light_Button{ x, 110, 25, 20 };
		btnB[i]->label("0");
		btnB[i]->color2(FL_RED);
		btnB[i]->callback(test1);

		btn_res[i] = new Fl_Light_Button{ x, 190, 25, 20 };
		btn_res[i]->label("0");
		btn_res[i]->color2(FL_RED);
		btn_res[i]->callback(results, btn_res[i]);

		aBox[i] = new Fl_Box{ x, 5, 25, 20, labels[i] };
		bBox[i] = new Fl_Box{ x, 90, 25, 20, labels[i] };
		cBox[i] = new Fl_Box{ x, 170, 25, 20, labels[i] };

		x -= 35;
	}
	btnres.callback(Calc, btn_res);
	window->end();
	window->show(argc, argv);
	return Fl::run();
}