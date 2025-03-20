/*! Digital Signal Processing Engine sockets usage example.
 *  Simple server application.
 * \author Marek Blok
 * \date 2010.02.09
 * \date updated 2021.01.18
 * \modified by Kamil Mycka to send a WAV file directly.
 */
#include <DSP_sockets.h>
#include <DSP_lib.h>
#include <DSP_modules_misc.h>

int main(void)
{
  // Tworzenie głównego zegara synchronizującego przetwarzanie sygnałów
  DSP::Clock_ptr MasterClock;
  int temp;
  long int Fp;

  MasterClock = DSP::Clock::CreateMasterClock();

  DSP::u::WaveInput AudioIn(MasterClock, "crab-rave.wav", ".");
  Fp = 44100; // Ustawienie częstotliwości próbkowania 44.1 kHz

  // Konfiguracja gniazda sieciowego (adres nasłuchu: wszystkie interfejsy, port 10000)
  std::string bind_address = "0.0.0.0:10000";
  DSP::u::SocketOutput out_socket(bind_address, false, 0x00000003);
  out_socket.SetName(bind_address); // Opcjonalne nazwanie komponentu

  // Podłączenie wyjścia audio do wejścia gniazda sieciowego
  AudioIn.Output("out") >> out_socket.Input("in");

  // Sprawdzenie poprawności połączeń między komponentami
  DSP::Component::CheckInputsOfAllComponents();

  // Generowanie pliku DOT z diagramem przetwarzania
  DSP::Clock::SchemeToDOTfile(MasterClock, "socket_server_wav.dot");

  // Oczekiwanie na połączenie klienta
  out_socket.WaitForConnection();

  // Główna pętla przetwarzania danych
  temp = 1;
  int chunk_size = Fp / 4; // Rozmiar porcji danych: 44100/4 = 11025 próbek (0.25s audio)
  do
  {
    // Przetworzenie porcji danych (wykonanie 11025 próbek)
    DSP::Clock::Execute(MasterClock, chunk_size);
    std::cout << MasterClock; // Wypisanie stanu zegara

    // Logowanie postępu przetwarzania
    DSP::log << "MAIN" << DSP::e::LogMode::second << temp << std::endl;
    temp++;
  }
  while (temp<60); // Wykonaj ~15 sekund przetwarzania (59 * 0.25s = 14.75s)

  // Zwolnienie zasobów zegarów i komponentów
  DSP::Clock::FreeClocks();

  // Finalny komunikat w logu
  DSP::log << "MAIN" << DSP::e::LogMode::second << "end" << std::endl;

  return 0;
}