import os
import subprocess
import shutil
import unittest

class TestMeshFixCLI(unittest.TestCase):
    executable = os.path.abspath("./bin64/MeshFix") if os.path.exists("./bin64/MeshFix") else os.path.abspath("./bin/MeshFix")
    test_dir = os.path.abspath("tests/tmp")
    sample_obj = os.path.abspath("bin64/MariaImmaculata_C_fixed.obj")

    @classmethod
    def setUpClass(cls):
        if not os.path.exists(cls.test_dir):
            os.makedirs(cls.test_dir)
        # Ensure we have a sample file for testing
        if not os.path.exists(cls.sample_obj):
            raise unittest.SkipTest("Sample OBJ file not found for tests")

    @classmethod
    def tearDownClass(cls):
        if os.path.exists(cls.test_dir):
            shutil.rmtree(cls.test_dir)

    def test_help_option(self):
        """Test if -h option displays help message."""
        result = subprocess.run([self.executable, "-h"], input=b"\n", capture_output=True)
        self.assertEqual(result.returncode, 0)
        self.assertIn(b"Usage: MeshFix", result.stdout)
        self.assertIn(b"Option '-h' = display this help", result.stdout)
        self.assertIn(b"Option '-o' = specify output file", result.stdout)

    def test_output_option(self):
        """Test if -o option correctly saves the output file."""
        output_file = os.path.join(self.test_dir, "output_with_o.off")
        if os.path.exists(output_file):
            os.remove(output_file)
        
        result = subprocess.run([self.executable, self.sample_obj, "-o", output_file], capture_output=True)
        self.assertEqual(result.returncode, 0)
        self.assertTrue(os.path.exists(output_file))
        self.assertGreater(os.path.getsize(output_file), 0)

    def test_positional_output(self):
        """Test if positional output argument still works."""
        output_file = os.path.join(self.test_dir, "output_positional.off")
        if os.path.exists(output_file):
            os.remove(output_file)
        
        result = subprocess.run([self.executable, self.sample_obj, output_file], capture_output=True)
        self.assertEqual(result.returncode, 0)
        self.assertTrue(os.path.exists(output_file))
        self.assertGreater(os.path.getsize(output_file), 0)

    def test_default_output(self):
        """Test default output filename generation."""
        # Copy sample to test dir to avoid cluttering bin64
        test_sample = os.path.join(self.test_dir, "sample.obj")
        shutil.copy(self.sample_obj, test_sample)
        expected_output = os.path.join(self.test_dir, "sample_fixed.off")
        
        if os.path.exists(expected_output):
            os.remove(expected_output)
            
        result = subprocess.run([self.executable, test_sample], capture_output=True)
        self.assertEqual(result.returncode, 0)
        self.assertTrue(os.path.exists(expected_output))

    def test_stl_output_option(self):
        """Test if -j option forces STL output."""
        output_file = os.path.join(self.test_dir, "output.stl")
        if os.path.exists(output_file):
            os.remove(output_file)
        
        result = subprocess.run([self.executable, self.sample_obj, "-o", output_file, "-j"], capture_output=True)
        self.assertEqual(result.returncode, 0)
        self.assertTrue(os.path.exists(output_file))
        # Basic check for STL format (binary or ASCII)
        with open(output_file, 'rb') as f:
            header = f.read(80)
            self.assertTrue(header.startswith(b'solid') or len(header) == 80)

if __name__ == "__main__":
    unittest.main()
