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
  DSP::Clock_ptr MasterClock;
  int temp;
  long int Fp;

  // Create the master clock
  MasterClock = DSP::Clock::CreateMasterClock();

  // Load the WAV file
  DSP::u::WaveInput AudioIn(MasterClock, "crab-rave.wav", ".");
  Fp = AudioIn.GetSamplingRate();

  // Use server socket to send the audio data
  std::string bind_address = "0.0.0.0:10000";
  DSP::u::SocketOutput out_socket(bind_address, false, 0x00000003);
  out_socket.SetName(bind_address);

  // Connect the WAV file output to the socket input
  AudioIn.Output("out") >> out_socket.Input("in");

  // Check if all components are properly connected
  DSP::Component::CheckInputsOfAllComponents();

  // Generate a DOT file to visualize the processing scheme
  DSP::Clock::SchemeToDOTfile(MasterClock, "socket_server_wav.dot");

  // Wait for a client to connect
  out_socket.WaitForConnection();

  // Process and send the audio data
  temp = 1;
  long int prev_bytes = 0;
  long int current_bytes;
  int chunk_size = Fp / 8; 
  do
  {
    DSP::Clock::Execute(MasterClock, chunk_size);

    current_bytes = AudioIn.GetBytesRead();
    if (current_bytes == prev_bytes)
    {
      // No new data processed, exit loop
      break;
    }
    prev_bytes = current_bytes;

    DSP::log << "MAIN" << DSP::e::LogMode::second << temp << std::endl;
    temp++;
  }
  while (true); // Loop until no more data

  // Optional: Close the socket explicitly if supported
  // out_socket.Close();

  // Free the clocks and clean up
  DSP::Clock::FreeClocks();

  DSP::log << "MAIN" << DSP::e::LogMode::second << "end" << std::endl;

  return 0;
}