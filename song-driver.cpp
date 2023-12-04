#include <string>
using namespace std;

#include "song.h"
#include "wavfilewriter.h"

const string INPUT_FILE = "D:\\input.txt";
const string OUTPUT_FILE = "D:\\output.wav";

int main()
{
  //Read in song from file and print
  Song song(INPUT_FILE);
  cout << song << endl;

  //Write the song to a WAV file
  WAVFileWriter writer(OUTPUT_FILE);
  writer.setSong(song);
  writer.writeToFile();

  return 0;
}