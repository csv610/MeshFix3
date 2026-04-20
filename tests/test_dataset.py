import os
import subprocess
import unittest

class TestMeshFixDataset(unittest.TestCase):
    executable = os.path.abspath("./bin/MeshFix")
    dataset_dir = os.path.abspath("dataset")
    output_dir = os.path.abspath("tests/dataset_output")

    @classmethod
    def setUpClass(cls):
        if not os.path.exists(cls.output_dir):
            os.makedirs(cls.output_dir)

    def run_meshfix(self, input_file):
        base_name = os.path.basename(input_file)
        output_file = os.path.join(self.output_dir, f"fixed_{base_name}")
        result = subprocess.run([self.executable, input_file, "-o", output_file], capture_output=True, text=True)
        return result, output_file

    def test_hole_filling(self):
        input_file = os.path.join(self.dataset_dir, "hole.off")
        result, output_file = self.run_meshfix(input_file)
        self.assertEqual(result.returncode, 0)
        self.assertIn("Patching holes", result.stdout + result.stderr)
        self.assertTrue(os.path.exists(output_file))

    def test_degenerate_needle(self):
        input_file = os.path.join(self.dataset_dir, "degenerate_needle.off")
        result, output_file = self.run_meshfix(input_file)
        self.assertEqual(result.returncode, 0)
        # For a single degenerate triangle, MeshFix might remove it or patch it.
        # Ensure it ran successfully.
        self.assertTrue(os.path.exists(output_file))

    def test_degenerate_cap(self):
        input_file = os.path.join(self.dataset_dir, "degenerate_cap.off")
        result, output_file = self.run_meshfix(input_file)
        self.assertEqual(result.returncode, 0)
        self.assertTrue(os.path.exists(output_file))

    def test_noise_removal(self):
        input_file = os.path.join(self.dataset_dir, "noise.off")
        result, output_file = self.run_meshfix(input_file)
        self.assertEqual(result.returncode, 0)
        # It should detect small components
        self.assertIn("small components", result.stdout + result.stderr)

    def test_intersection_removal(self):
        input_file = os.path.join(self.dataset_dir, "intersect.off")
        result, output_file = self.run_meshfix(input_file)
        self.assertEqual(result.returncode, 0)
        # For 2 triangles, it might just remove one as a small component
        self.assertTrue(os.path.exists(output_file))

if __name__ == "__main__":
    unittest.main()
