#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>  
#include <algorithm>

#include "Song.h"

using namespace std;

Song::Song(const string& file)
{
	string line;
	std::ifstream FILE(file);
	vector<string> words;

	std::getline(FILE, line);
	setTempo(stoi(line));
	while (std::getline(FILE, line)) {
		voices.push_back(Voice(line));
	}
	FILE.close();
}

vector<string> split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream. 
	string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}

Note::Note(string str) {
	vector<char> pitchs; 
	vector<char> duration_s = { 'q','h','w','e','s'};
	vector<double> duration_d = { 1, 2, 4, 0.5, 0.25 };
	double duration_val = 0;
	const char* chars = str.c_str();
	int i = 0;
	//convert a note from string i.e. "D4h.", "F#2q"
	for (; *chars; ++chars)
	{
		if ((i == 0) || (i == 1 && !isdigit(*chars))) 
		{
			pitchs.push_back(*chars);
		}
		else if (isdigit(*chars)) {
			octave = *chars - 48;
		}
		else if (i > 1)
		{
			int x = -1;
			auto it = find(duration_s.begin(), duration_s.end(), *chars);
			if (it != duration_s.end()) {
				x = distance(duration_s.begin(), it);
			}
			if (x >= 0) {
				duration_val = duration_d[x];
			}
			else if (x == -1 && *chars == '.') {
				duration_val *= 1.5;
			}
		}
		i++;
	}

	duration = duration_val;	
	std::string s(pitchs.begin(), pitchs.end());
	setPitch(s);
}

//function to set pitch as per the enum ptcih_t
void Note::setPitch(string s) {
	// C, Db, D, Eb, E, F, Gb, G, Ab, A, Bb, B, REST
	if (s == "A") pitch = A;
	if (s == "B") pitch = B;
	if (s == "C") pitch = C;
	if (s == "D") pitch = D;
	if (s == "E") pitch = E;
	if (s == "F") pitch = F;
	if (s == "G") pitch = G;

	if (s == "A#" || s == "Bb") pitch = Bb;
	if (s == "C#" || s == "Db") pitch = Db;
	if (s == "D#" || s == "Eb") pitch = Eb;
	if (s == "F#" || s == "Gb") pitch = Gb;
	if (s == "G#" || s == "Ab") pitch = Ab;

	if (s == "R") pitch = REST;
}

//get durationstring from duration
//user for print function only
string Note::getDurationString(double duration) const {
	string return_val = "";
	double d = duration / 1.5;
	if (duration == 1 || d == 1) return_val = "q";
	else if (duration == 2 || d == 2) return_val = "h";
	else if (duration == 4 || d == 4) return_val = "w";
	else if (duration == 0.5 || d == 0.5) return_val = "e";
	else if (duration == 0.25 || d == 0.25) return_val = "s";
	if (d == 1 || d == 2 || d == 4 || d == 0.5 || d == 0.25)
		return_val = return_val + ".";

	return return_val;
}

Voice::Voice(const string& str, double vol, instrument_t instr) {
	volume = vol;
	instrument = instr;
	std::istringstream iss(str);
	for (std::string s; iss >> s; ) {
		Note n = Note(s);
		notes.push_back(n);
	}
}

//Print function for Songs
//Should print in same format as input file
ostream& operator<<(ostream& out, const Song& song) {
	vector<Voice> voices = song.getVoices();
	out << song.getTempo() << endl;
	for (Voice x : voices)		
		out << x << endl;
	return out;
}

//Print function for Voices
//Should print all notes in the voice, separated by spaces
ostream& operator<<(ostream& out, const Voice& voice) {
	vector<Note> notes = voice.getNotes();
	for (Note x : notes)
		out << x << " ";
	return out;
}

//Print function for Notes
//Should print a note in the same format as the input file;
// e.g., C3q or Db4e.
ostream& operator<<(ostream& out, const Note& note) {
	out << note.getPitch();;
	out << note.getOctave();
	out << note.getDurationString(note.getDuration());

	return out;
}

ostream& operator<<(ostream& out, const pitch_t& p) {
	// C, Db, D, Eb, E, F, Gb, G, Ab, A, Bb, B, REST
	switch (p) {
		case A: out << "A"; break;
		case B: out << "B"; break;
		case C: out << "C"; break;
		case D: out << "D"; break;
		case E: out << "E"; break;
		case F: out << "F"; break;
		case G: out << "G"; break;

		case Bb: out << "Bb"; break;
		case Db: out << "Db"; break;
		case Eb: out << "Eb"; break;
		case Gb: out << "Gb"; break;
		case Ab: out << "Ab"; break;

		default: out << "R"; break;
		//PAOK
	}
	return out;
}

