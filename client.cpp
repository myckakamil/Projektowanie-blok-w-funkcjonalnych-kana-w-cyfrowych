/*! Digital Signal Processing Engine sockets usage example.
 *  Prosta aplikacja kliencka (2) do odbioru i zapisu pliku WAV.
 * \author Marek Blok
 * \date 2010.02.09
 * \date updated 2021.01.18
 * \modified by Kamil Mycka to receive and save a WAV file.
 */
#include <DSP_sockets.h>
#include <DSP_lib.h>

int main(void)
{
  DSP::Clock_ptr MasterClock;
  int temp;

  // Konfiguracja systemu logowania (konsola + plik)
  DSP::log.SetLogState(DSP::e::LogState::console | DSP::e::LogState::file);
  DSP::log.SetLogFileName("log_file_client.log");  // Plik logu
  DSP::log << DSP::lib_version_string() << std::endl << std::endl;  // Wersja biblioteki

  // Tworzenie głównego zegara przetwarzania sygnałów
  MasterClock = DSP::Clock::CreateMasterClock();

  std::string server_address = "127.0.0.1:10000";
  DSP::u::SocketInput in_socket(MasterClock, server_address, true, 0x00000003);
  in_socket.SetName(server_address); 

  int Fp = 44100;  // Częstotliwość próbkowania MUSI zgadzać się z serwerem!
  DSP::u::FileOutput WAVEfile("received_crab_rave.wav", DSP::e::SampleType::ST_short, 1, DSP::e::FileType::FT_wav, Fp);
  
  // Podłączenie wyjścia gniazda sieciowego do wejścia pliku WAV
  in_socket.Output("out") >> WAVEfile.Input("in");

  // Weryfikacja poprawności połączeń między komponentami
  DSP::Component::CheckInputsOfAllComponents();

  // Generowanie pliku DOT z diagramem przetwarzania
  DSP::Clock::SchemeToDOTfile(MasterClock, "socket_client_wav.dot");

  // Główna pętla przetwarzania danych
  temp = 1;
  long int prev_bytes = 0;
  long int current_bytes;
  do
  {
    // Przetwarzanie porcji danych (44100/4 = 11025 próbek ~0.25s audio)
    DSP::Clock::Execute(MasterClock, Fp /4 );

    // Logowanie postępu z liczbą odebranych bajtów
    DSP::log << "MAIN" << DSP::e::LogMode::second << temp 
             << " (" << in_socket.GetBytesRead() << ")" << std::endl;
    temp++;
  }
  while (in_socket.GetBytesRead() != 0);  // Pętla aż do końca transmisji

  // Zwolnienie zasobów systemowych i zamknięcie plików
  DSP::Clock::FreeClocks();

  return 0;
}