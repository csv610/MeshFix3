#include "tmesh.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <filesystem>
#include <string>
#include <string_view>

namespace fs = std::filesystem;
using namespace T_MESH;
void usage()
{
 printf("\nMeshFix3 \n------\n");
 printf("Usage: MeshFix inmeshfile [outmeshfile] [-a] [-j] [-x] [-o outmeshfile] [-h]\n");
 printf("  Processes 'inmeshfile' and saves the result to 'outmeshfile'\n");
 printf("  If 'outmeshfile' is not specified '<input-dir>/<input-name>_fixed.off' will be produced\n");
 printf("  Option '-o' = specify output file\n");
 printf("  Option '-h' = display this help\n");
 printf("  Option '-a' = joins multiple open components before starting\n");
 printf("  Option '-j' = output files in STL format insted of OFF\n");
 printf("  Option '-x' exits if output file already exists.\n");
 printf("  Accepted input formats are OFF, PLY and STL.\n  Other formats are supported only partially.\n");
 exit(0);
}

std::string createFilename(const std::string& iname, const std::string& subext, const std::string& newextension)
{
    fs::path p = fs::absolute(fs::path(iname));
    return (p.parent_path() / (p.stem().string() + subext + newextension)).string();
}

void printRepairSummary(const RepairReport& report)
{
	printf("Repair summary:\n");
	printf("  joined components: %d\n", report.joined_components);
	printf("  removed components: %d\n", report.removed_components);
	printf("  patched holes: %d\n", report.patched_holes);
	printf("  exact degeneracies: %d -> %d\n", report.degeneracies_before, report.degeneracies_after);
	printf("  intersecting triangles: %d -> %d\n", report.intersections_before, report.intersections_after);
}

int main(int argc, char *argv[])
{
 TMesh::init(); // This is mandatory
 TMesh::app_name = "MeshFix";
 TMesh::app_version = "2.0";
 TMesh::app_year = "2016";
 TMesh::app_authors = "Marco Attene";
 TMesh::app_maillist = "attene@ge.imati.cnr.it";

 clock_t beginning = clock();

 // Uncomment the following to prevent message reporting
 // TMesh::quiet = true;

 Basic_TMesh tin;
 bool stl_output = false;
 bool skip_if_fixed = false;
 bool join_multiple_components = false;
 std::string infilename, outfilename;
 std::string extension = ".off";
 bool output_specified = false;

 for (int i = 1; i < argc; i++)
 {
  std::string_view arg = argv[i];
  if (arg == "-h") usage();
  else if (arg == "-x") skip_if_fixed = true;
  else if (arg == "-a") join_multiple_components = true;
  else if (arg == "-j") stl_output = true;
  else if (arg == "-o")
  {
   if (i + 1 < argc) {
    outfilename = argv[++i];
    output_specified = true;
   }
   else TMesh::error("-o option requires an argument.\n");
  }
  else if (arg.starts_with('-')) TMesh::warning("%s - Unknown operation.\n", argv[i]);
  else {
   if (infilename.empty()) infilename = argv[i];
   else if (!output_specified) {
    outfilename = argv[i];
    output_specified = true;
   }
  }
 }

 if (infilename.empty()) usage();

 if (stl_output) extension = ".stl";
 if (!output_specified) outfilename = createFilename(infilename, "_fixed", extension);

 fs::path input_path(infilename);
 fs::path output_path(outfilename);

 if (skip_if_fixed && fs::exists(outfilename)) TMesh::error("Output file already exists (-x option specified).");
 if (!fs::exists(input_path)) TMesh::error("Input file does not exist.\n");
 if (!fs::is_regular_file(input_path)) TMesh::error("Input path is not a regular file.\n");
 if (output_path.has_parent_path() && !fs::exists(output_path.parent_path())) TMesh::error("Output directory does not exist.\n");

 // The loader automatically reconstructs a manifold triangle connectivity
 if (tin.load(infilename.c_str()) != 0) TMesh::error("Can't open file.\n");

	 RepairReport report;

	 RepairOptions options{.join_multiple_components = join_multiple_components};
	 tin.repair(options, report);

 printRepairSummary(report);
	
 if (!report.repairedSomething())
 {
		 TMesh::info("No repair was applied. No output written.\n");
	 if (CLOCKS_PER_SEC >= 1000)
		 printf("Elapsed time: %lu ms\n", (clock() - beginning) / (CLOCKS_PER_SEC / 1000));
	 return 0;
 }

	
 TMesh::info("Saving output mesh ...\n");
 if (tin.save(outfilename.c_str()) != 0) TMesh::error("Failed to save output mesh.\n");

 // Normally, CLOCKS_PER_SEC >= 1000, but this is not technically guaranteed,
 // and if it were not true, dividing by zero would invoke undefined behavior.
 if (CLOCKS_PER_SEC >= 1000)
	 printf("Elapsed time: %lu ms\n", (clock() - beginning) / (CLOCKS_PER_SEC / 1000));

 return 0;
}
