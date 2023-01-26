#include <iostream>
#include <string>
#include <cstring>
#include <filesystem>
#include <thread>
#include <mutex>
#include <queue>

#include "ptx_structures/ptx_program.h"
#include "parser_tools.h"
#include "data_structures/options.h"
#include "logger.h"

void write_to_terminal(const std::string &filename, PtxProgram &pp) {
  std::cout << "Filename: " << filename << std::endl;
  std::cout << "PTX Version: " << pp.version_ << std::endl;
  std::cout << "PTX Target: " << pp.target_ << std::endl;
  std::cout << "PTX Address Size: " << pp.address_size_ << std::endl;
  std::cout << std::endl;

  std::cout << "Instructions written in PTX File: " << pp.instructions_ << std::endl;
  std::cout << "Weighted Instructions written in PTX File: " << pp.weighted_instructions_ << std::endl;
  std::cout << std::endl;

  std::cout << "CTAs: " << pp.GetCTACount() << std::endl;
  std::cout << "Thread Count: " << pp.threads_ << std::endl;
  std::cout << "Total Number of Floating Point Instructions: " << pp.GetTotalFPI() << std::endl;
  std::cout << "Total Number of Instructions: " << pp.GetInstructionsAllThreads() << std::endl;
  std::cout << std::endl;

  std::cout << "Total Uses of Registers with type \"reg\": " << pp.GetNumberRegisters(kRegister) << std::endl;
  std::cout << std::endl;

  std::cout << "Number of Instructions to simulate: " << pp.GetNumberOfInstructionsToExecute() << " -> "
            << (((float) pp.GetNumberOfInstructionsToExecute()) / ((float) pp.instructions_)) * 100 << "%" << std::endl;

  if (!Options::static_mode) {
    unsigned int i = 0;
    InstructionCounts result;
    for (auto p = pp.parts_.begin(); p != pp.parts_.end(); ++p, ++i) {
      if (Options::verbose) {
        Logger::Log("main.cc:write_to_terminal | Starting " + filename + " Part: " + std::to_string(i) + "\n", "DEBUG");
      }
      InstructionCounts c = p->Execute();
      result.total_ic += c.total_ic;
      result.fp_ic += c.fp_ic;
    }
    std::cout << "Dynamic Instruction Count: " << result.total_ic << "\n";
    std::cout << "FP Instructions (Dynamic): " << result.fp_ic << "\n";
  }
}

void write_to_csv(const std::string &filename, PtxProgram &pp, auto &start_static) {
  std::string output = "\"" + filename + "\";";
  output += "\"" + pp.version_ + "\";";
  output += "\"" + pp.target_ + "\";";
  output += "\"" + pp.address_size_ + "\";";
  output += "\"" + std::to_string(pp.instructions_) + "\";";
  output += "\"" + std::to_string(pp.GetCTACount()) + "\";";
  output += "\"" + std::to_string(pp.threads_) + "\";";
  output += "\"" + std::to_string(pp.GetInstructionsAllThreads()) + "\"";

  auto end_static = std::chrono::high_resolution_clock::now();
  auto duration_static = duration_cast<std::chrono::milliseconds>(end_static - start_static);
  if (Options::verbose) {
    Logger::Log("main:write_to_csv | Static Duration: " + std::to_string(duration_static.count()) + "ms", "DEBUG");
  }

  if (!Options::static_mode) {
    auto start_dynamic = std::chrono::high_resolution_clock::now();
    unsigned int i = 0;
    InstructionCounts result;
    for (auto p = pp.parts_.begin(); p != pp.parts_.end(); ++p, ++i) {
      if (Options::verbose) {
        Logger::Log("main.cc:write_to_csv | Starting " + filename + " Part: " + std::to_string(i) + "\n", "DEBUG");
      }
      InstructionCounts c = p->Execute();
      result.total_ic += c.total_ic;
      result.fp_ic += c.fp_ic;
      result.exec_ic += c.exec_ic;
      result.divergent_branches += c.divergent_branches;
      for (const auto &i : c.db_list) {
        result.db_list.push_back(i);
      }
    }
    output += ";\"" + std::to_string(result.total_ic) + "\"";
    output += ";\"" + std::to_string(result.fp_ic) + "\"";
    output += ";\"" + std::to_string(result.exec_ic) + "\"";
    output +=
        ";\"" + std::to_string((static_cast<float>(result.exec_ic) / static_cast<float>(result.total_ic)) * 100) + "\"";
    output += ";\"" + std::to_string(result.divergent_branches) + "\"";
    output += ";\"";
    for (const auto &i : result.db_list) {
      output += std::to_string(i);
      output += ",";
    }
    output += "\"";

    auto end_dynamic = std::chrono::high_resolution_clock::now();
    auto duration_dynamic = duration_cast<std::chrono::milliseconds>(end_dynamic - start_dynamic);
    auto duration_total = duration_cast<std::chrono::milliseconds>(end_dynamic - start_static);
    output += ";\"" + std::to_string(duration_dynamic.count()) + "\"";
    if (Options::verbose) {
      Logger::Log("main:write_to_csv | Dynamic Duration: " + std::to_string(duration_dynamic.count()) + "ms", "DEBUG");
      Logger::Log("main:write_to_csv | Total Duration: " + std::to_string(duration_total.count()) + "ms", "DEBUG");
    }
  }

  output += "\n";

  Logger::Print(output);
}

void handle_filename(const std::string &filename) {
  auto start_static = std::chrono::high_resolution_clock::now();
  std::ifstream ptx_file(filename);

  PtxProgram pp;

  if (ptx_file.is_open()) {
    std::string line;
    while (ptx_file) {
      std::getline(ptx_file, line);
      Trim(line);
      if (!line.empty() && !IsComment(line)) {
        handle_line(line, pp);
      }
    }
  }

  ptx_file.close();

  if (Options::graphs) {
    std::string graph_path("graphs/");

    size_t pos = filename.find_last_of('/');
    if (pos == std::string::npos) {
      graph_path += filename;
    } else {
      graph_path += filename.substr(pos + 1);
    }

    pp.ExportGraphs(graph_path);
  }

  if (Options::csv) {
    write_to_csv(filename, pp, start_static);
  } else {
    write_to_terminal(filename, pp);
  }

}

void thread_runner(std::queue<std::string> &files, std::mutex &mtx) {
  while (true) {
    mtx.lock();
    if (files.empty()) {
      mtx.unlock();
      break;
    } else {
      std::string filename = files.front();
      files.pop();
      mtx.unlock();

      handle_filename(filename);
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " [files]..." << std::endl;
  } else {
    std::map<std::string, bool *> cli_ops;
    cli_ops["-s"] = &Options::static_mode;
    cli_ops["-ot"] = &Options::one_thread;
    cli_ops["-csv"] = &Options::csv;
    cli_ops["-g"] = &Options::graphs;
    cli_ops["-v"] = &Options::verbose;

    for (int i = 1; i < argc; i++) {
      if (cli_ops.find(argv[i]) != cli_ops.end()) {
        (*cli_ops[argv[i]]) = true;
      }
    }

    Logger::OpenFile();

    // CSV Header
    if (Options::csv) {
      if (!Options::static_mode) {
        Logger::Print(
            "\"Filename\";\"Ptx Version\";\"Ptx Target\";\"Ptx Adress Size\";\"File Instructions\";\"CTAs\";\"Thread Count\";\"Static Instructions\";\"Dynamic Instruction Count\";\"FP Instructions\";\"Executed Instructions\";\"(Executed instructions/Dynamic Instruction Count)*100\";\"Divergent Branches\";\"Divergent Branches for each Part\";\"Duration (msec)\"\n");
      } else {
        Logger::Print(
            "\"Filename\";\"Ptx Version\";\"Ptx Target\";\"Ptx Adress Size\";\"File Instructions\";\"CTAs\";\"Thread Count\";\"Static Instructions\";\n");
      }
    }

    std::queue<std::string> files;
    for (int i = 1; i < argc; i++) {
      if (cli_ops.find(argv[i]) == cli_ops.end()) {
        if (std::filesystem::is_directory(argv[i])) {
          for (const auto &file : std::filesystem::recursive_directory_iterator(argv[i])) {
            if (file.path().extension() == ".ptx" && !std::filesystem::is_directory(file)) {
              files.push(file.path());
            }
          }
        } else {
          files.push(argv[i]);
        }
      }
    }

    auto nproc = std::thread::hardware_concurrency();
    std::thread threads[nproc];
    std::mutex mtx;

    for (unsigned int i = 0; i < nproc; i++) {
      threads[i] = std::move(std::thread(thread_runner, std::ref(files), std::ref(mtx)));
    }

    for (auto &t : threads) {
      t.join();
    }

    Logger::CloseFile();
  }
  return 0;
}

