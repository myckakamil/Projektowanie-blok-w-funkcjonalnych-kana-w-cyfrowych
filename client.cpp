/*! Digital Signal Processing Engine sockets usage example.
 *  Simple client application (2).
 * \author Marek Blok
 * \date 2010.02.09
 * \date updated 2021.01.18
 * \modified by [Your Name] to receive and save a WAV file.
 */
#include <DSP_sockets.h>
#include <DSP_lib.h>

int main(void)
{
  DSP::Clock_ptr MasterClock;
  int temp;

  // Configure logging
  DSP::log.SetLogState(DSP::e::LogState::console | DSP::e::LogState::file);
  DSP::log.SetLogFileName("log_file_client.log");

  DSP::log << DSP::lib_version_string() << std::endl << std::endl;

  // Create the master clock
  MasterClock = DSP::Clock::CreateMasterClock();

  // Use client socket to connect to the server
  std::string server_address = "127.0.0.1:10000";  // Server address
  DSP::u::SocketInput in_socket(MasterClock, server_address, true, 0x00000003);
  in_socket.SetName(server_address);

  // Save received data to a WAV file
  DSP::u::FileOutput WAVEfile("received_crab_rave.wav", DSP::e::SampleType::ST_short, 1, DSP::e::FileType::FT_wav, 8000);
  in_socket.Output("out") >> WAVEfile.Input("in");

  // Optionally, connect to an audio output for real-time playback
  // DSP::u::AudioOutput AudioOut(8000);
  // in_socket.Output("out") >> AudioOut.Input("in");

  // Check if all components are properly connected
  DSP::Component::CheckInputsOfAllComponents();

  // Optionally, generate a DOT file to visualize the processing scheme
  DSP::Clock::SchemeToDOTfile(MasterClock, "socket_client_wav.dot");

  // Process incoming data
  temp = 1;
  do
  {
    // Execute the processing for a chunk of data
    DSP::Clock::Execute(MasterClock, 44100 / 4);

    // Log the current iteration and bytes read
    DSP::log << "MAIN" << DSP::e::LogMode::second << temp 
             << " (" << in_socket.GetBytesRead() << " bytes received)" << std::endl;
    temp++;
  }
  while (in_socket.GetBytesRead() != 0);  // Continue until all data is received

  // Free the clocks and clean up
  DSP::Clock::FreeClocks();

  return 0;
}