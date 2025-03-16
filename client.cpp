/*! Digital Signal Processing Engine sockets usage example.
 *  Simple client application (2).
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

  int Fp = 44100; // Must match the server's file sampling rate
  DSP::u::FileOutput WAVEfile("received_crab_rave.wav", DSP::e::SampleType::ST_short, 1, DSP::e::FileType::FT_wav, Fp);
  in_socket.Output("out") >> WAVEfile.Input("in");

  // Check if all components are properly connected
  DSP::Component::CheckInputsOfAllComponents();

  // Generate a DOT file to visualize the processing scheme
  DSP::Clock::SchemeToDOTfile(MasterClock, "socket_client_wav.dot");

  // Process incoming data
  temp = 1;
  long int prev_bytes = 0;
  long int current_bytes;
  int chunk_size = Fp / 8; // Match the server's chunk size
  do
  {
    // Execute the processing for a chunk of data
    DSP::Clock::Execute(MasterClock, chunk_size);

    current_bytes = in_socket.GetBytesRead();
    if (current_bytes == prev_bytes)
    {
      // No new data received, exit loop
      break;
    }
    prev_bytes = current_bytes;

    // Log the current iteration and bytes read
    DSP::log << "MAIN" << DSP::e::LogMode::second << temp 
             << " (" << current_bytes << " bytes received)" << std::endl;
    temp++;
  }
  while (true);  // Loop until no new data

  // Free the clocks and clean up
  DSP::Clock::FreeClocks();

  return 0;
}