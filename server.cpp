/*! Digital Signal Processing Engine sockets usage example.
 *  Simple server application.
 * \author Marek Blok
 * \date 2010.02.09
 * \date updated 2021.01.18
 * \modified by [Your Name] to send a WAV file directly.
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
  Fp = AudioIn.GetSamplingRate(); // Get the sampling rate from the WAV file

  // Use server socket to send the audio data
  std::string bind_address = "0.0.0.0:10000";
  DSP::u::SocketOutput out_socket(bind_address, false, 0x00000003);
  out_socket.SetName(bind_address);

  // Connect the WAV file output to the socket input
  AudioIn.Output("out") >> out_socket.Input("in");

  // Check if all components are properly connected
  DSP::Component::CheckInputsOfAllComponents();

  // Optionally, generate a DOT file to visualize the processing scheme
  DSP::Clock::SchemeToDOTfile(MasterClock, "socket_server_wav.dot");

  // Wait for a client to connect
  out_socket.WaitForConnection();

  // Process and send the audio data
  temp = 1;
  do
  {
    // Execute the processing for a chunk of data
    DSP::Clock::Execute(MasterClock, Fp / 8);

    // Log the current iteration
    DSP::log << "MAIN" << DSP::e::LogMode::second << temp << std::endl;
    temp++;
  }
  while (AudioIn.GetBytesRead() != 0); // Continue until the entire file is sent

  // Free the clocks and clean up
  DSP::Clock::FreeClocks();

  // Log the end of the program
  DSP::log << DSP::e::LogMode::Error << "MAIN" << DSP::e::LogMode::second << "end" << std::endl;

  return 0;
}