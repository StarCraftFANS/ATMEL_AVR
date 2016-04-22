#define	MORSE(pat_,len_)	(pat_*8|len_)

PROGMEM uchar morse_tab[26+10]={
//	MORSE(0b00000	,0),	// �A���t�@�x�b�g
	MORSE(0b10000	,2),	// �`.-
	MORSE(0b01110	,4),	// �a-...
	MORSE(0b01010	,4),	// �b-.-.
	MORSE(0b01100	,3),	// �c-..
	MORSE(0b10000	,1),	// �d.
	MORSE(0b11010	,4),	// �e..-.
	MORSE(0b00100	,3),	// �f--.
	MORSE(0b11110	,4),	// �g....
	MORSE(0b11000	,2),	// �h..
	MORSE(0b10000	,4),	// �i.---
	MORSE(0b01000	,3),	// �j-.-
	MORSE(0b10110	,4),	// �k.-..
	MORSE(0b00000	,2),	// �l--
	MORSE(0b01000	,2),	// �m-.
	MORSE(0b00000	,3),	// �n---
	MORSE(0b10010	,4),	// �o.--.
	MORSE(0b00100	,4),	// �p--.-
	MORSE(0b10100	,3),	// �q.-.
	MORSE(0b11100	,3),	// �r...
	MORSE(0b00000	,1),	// �s-
	MORSE(0b11000	,3),	// �t..-
	MORSE(0b11100	,4),	// �u...-
	MORSE(0b10000	,3),	// �v.--
	MORSE(0b01100	,4),	// �w-..-
	MORSE(0b01000	,4),	// �x-.--
	MORSE(0b00110	,4),	// �y--..
//	MORSE(0b00000	,0),	// ����
	MORSE(0b00000	,5),	// �O-----
	MORSE(0b10000	,5),	// �P.----
	MORSE(0b11000	,5),	// �Q..---
	MORSE(0b11100	,5),	// �R...--
	MORSE(0b11110	,5),	// �S....-
	MORSE(0b11111	,5),	// �T.....
	MORSE(0b01111	,5),	// �U-....
	MORSE(0b00111	,5),	// �V--...
	MORSE(0b00011	,5),	// �W---..
	MORSE(0b00001	,5),	// �X----.
//	MORSE(0b00000	,0),	// 
};

