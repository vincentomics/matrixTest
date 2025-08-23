#!/usr/bin/env python3
"""
Convert matrix operations profiling logs to comprehensive markdown tables.
Supports C++, NumPy, pandas, R dataframes, and R matrices.

This script can parse all log files and generate various markdown table formats:
- Single file with sections for each test size
- README-style summary tables
- Individual test size tables
- Complete comparison tables
"""

import re
import sys
import os
import argparse
from typing import Dict, List, Tuple, Optional

class LogParser:
    def __init__(self):
        # Language configurations
        self.language_configs = {
            'C++': {
                'file': 'log_cpp_mat.txt',
                'row_pattern': 'row-major',
                'col_pattern': 'col-major',
                'operation_pattern': r'(row-major|col-major)\s+(.+?)\s*:\s*([\d.]+)\s*±\s*([\d.]+)\s*seconds'
            },
            'NumPy': {
                'file': 'log_python_np.txt',
                'row_pattern': 'row-major',
                'col_pattern': 'col-major',
                'operation_pattern': r'(row-major|col-major)\s+(.+?)\s*:\s*([\d.]+)\s*±\s*([\d.]+)\s*seconds'
            },
            'pandas': {
                'file': 'log_python_pd.txt',
                'row_pattern': 'row-oriented',
                'col_pattern': 'col-oriented',
                'operation_pattern': r'(row-oriented|col-oriented)\s+(.+?)\s*:\s*([\d.]+)\s*±\s*([\d.]+)\s*seconds'
            },
            'R df': {
                'file': 'log_R_df.txt',
                'row_pattern': 'row-major',
                'col_pattern': 'col-major',
                'operation_pattern': r'(row-major|col-major)\s+(.+?)\s*:\s*([\d.]+)\s*±\s*([\d.]+)\s*seconds'
            },
            'R mat': {
                'file': 'log_R_mat.txt',
                'row_pattern': 'row-major',
                'col_pattern': 'col-major',
                'operation_pattern': r'(row-major|col-major)\s+(.+?)\s*:\s*([\d.]+)\s*±\s*([\d.]+)\s*seconds'
            }
        }
        
        # Standard operation names for consistency
        self.standard_operations = [
            'sum', 'mean', 'std', 'transpose', 'reshape', 'write to txt', 'read from txt'
        ]

    def normalize_operation_name(self, operation: str) -> str:
        """Normalize operation names to standard format."""
        operation = operation.strip()
        
        # Handle pandas special cases
        if 'reshape' in operation.lower():
            return 'reshape'
        
        # Map common variations
        for standard_op in self.standard_operations:
            if standard_op in operation.lower():
                return standard_op
        
        return operation

    def parse_log_file(self, filepath: str, config: Dict) -> Dict[str, Dict[str, Dict[str, str]]]:
        """Parse a single log file according to its configuration."""
        if not os.path.exists(filepath):
            print(f"Warning: {filepath} not found, skipping...")
            return {}
            
        with open(filepath, 'r') as f:
            content = f.read()
        
        tests = {}
        
        # Pattern to match test sections - handle different test naming conventions
        test_patterns = [
            r'--- (.*?Test.*?) ---.*?=== Multiple Operations Profiling ===(.*?)(?=---|$)',
            r'--- (.*?) ---.*?=== Multiple Operations Profiling ===(.*?)(?=---|$)'
        ]
        
        test_matches = []
        for pattern in test_patterns:
            matches = re.findall(pattern, content, re.DOTALL)
            if matches:
                test_matches = matches
                break
        
        for test_name, operations_text in test_matches:
            operations = {}
            
            # Use the language-specific operation pattern
            matches = re.findall(config['operation_pattern'], operations_text)
            
            for layout, operation, mean_time, std_time in matches:
                operation = self.normalize_operation_name(operation)
                time_str = f"{mean_time} ± {std_time}s"
                
                if operation not in operations:
                    operations[operation] = {'row': '', 'col': ''}
                
                if layout == config['row_pattern']:
                    operations[operation]['row'] = time_str
                else:  # col pattern
                    operations[operation]['col'] = time_str
            
            # Clean up test name
            test_name = test_name.strip()
            if 'DataFrame' in test_name:
                test_name = test_name.replace('DataFrame ', '')
            if 'Matrix' in test_name:
                test_name = test_name.replace('Matrix ', '')
            
            tests[test_name] = operations
        
        return tests

    def parse_all_logs(self) -> Dict[str, Dict[str, Dict[str, Dict[str, str]]]]:
        """Parse all log files and return organized data."""
        all_data = {}
        
        for language, config in self.language_configs.items():
            print(f"Parsing {language} log file: {config['file']}")
            language_data = self.parse_log_file(config['file'], config)
            if language_data:
                all_data[language] = language_data
                print(f"  Found {len(language_data)} tests")
            else:
                print(f"  No data found or file missing")
        
        return all_data

    def get_test_names(self, all_data: Dict) -> List[str]:
        """Get consistent test names across all languages."""
        test_names = set()
        for language_data in all_data.values():
            test_names.update(language_data.keys())
        
        # Sort by size (tiny, small, medium, large)
        size_order = ['tiny', 'small', 'medium', 'large']
        sorted_tests = []
        
        for size in size_order:
            for test_name in sorted(test_names):
                if size in test_name.lower() and test_name not in sorted_tests:
                    sorted_tests.append(test_name)
        
        # Add any remaining tests
        for test_name in sorted(test_names):
            if test_name not in sorted_tests:
                sorted_tests.append(test_name)
        
        return sorted_tests

    def format_comparison_table(self, all_data: Dict, test_name: str) -> str:
        """Format a comparison table for a specific test."""
        # Table header
        languages = ['C++', 'NumPy', 'pandas', 'R df', 'R mat']
        table = f"## {test_name}\n\n"
        table += "| Operation | Orientation | " + " | ".join(languages) + " |\n"
        table += "| --- | --- | " + " | ".join(["---"] * len(languages)) + " |\n"
        
        # Collect all operations from all languages for this test
        all_operations = set()
        for language in languages:
            if language in all_data and test_name in all_data[language]:
                all_operations.update(all_data[language][test_name].keys())
        
        # Sort operations in standard order
        sorted_operations = []
        for std_op in self.standard_operations:
            if std_op in all_operations:
                sorted_operations.append(std_op)
        
        # Add any remaining operations
        for op in sorted(all_operations):
            if op not in sorted_operations:
                sorted_operations.append(op)
        
        # Generate table rows
        for operation in sorted_operations:
            row = [operation, "row<br>col"]
            
            for language in languages:
                cell_content = ""
                if (language in all_data and 
                    test_name in all_data[language] and 
                    operation in all_data[language][test_name]):
                    
                    op_data = all_data[language][test_name][operation]
                    if op_data['row'] and op_data['col']:
                        cell_content = f"{op_data['row']}<br>{op_data['col']}"
                    elif op_data['row']:
                        cell_content = f"{op_data['row']}<br>-"
                    elif op_data['col']:
                        cell_content = f"-<br>{op_data['col']}"
                
                row.append(cell_content)
            
            table += "| " + " | ".join(row) + " |\n"
        
        return table

    def format_summary_table(self, all_data: Dict, test_name: str) -> str:
        """Format a single-row summary table for README-style comparison."""
        languages = ['C++', 'NumPy', 'pandas', 'R df', 'R mat']
        
        # Just show the tiny test in README format
        table = "| Operation | Orientation | " + " | ".join(languages) + " |\n"
        table += "| --- | --- | " + " | ".join(["---"] * len(languages)) + " |\n"
        
        for operation in self.standard_operations:
            row = [operation, "row<br>col"]
            
            for language in languages:
                cell_content = ""
                if (language in all_data and 
                    test_name in all_data[language] and 
                    operation in all_data[language][test_name]):
                    
                    op_data = all_data[language][test_name][operation]
                    if op_data['row'] and op_data['col']:
                        cell_content = f"{op_data['row']}<br>{op_data['col']}"
                
                row.append(cell_content)
            
            table += "| " + " | ".join(row) + " |\n"
        
        return table

def save_to_file(content: str, filename: str):
    """Save content to a file."""
    with open(filename, 'w') as f:
        f.write(content)
    print(f"Saved to {filename}")

def main():
    parser_args = argparse.ArgumentParser(
        description="Generate markdown tables from matrix operation logs",
        epilog="""
Examples:
  python matrix_log_parser.py --sections --save     # One file with all test sections
  python matrix_log_parser.py --readme-style --save # README-style summary table
  python matrix_log_parser.py --test tiny --save    # Specific test size only
  python matrix_log_parser.py --all                 # Generate all formats
        """,
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    
    parser_args.add_argument('--save', action='store_true', 
                            help='Save output to markdown files instead of printing to console')
    parser_args.add_argument('--test', type=str, choices=['tiny', 'small', 'medium', 'large'], 
                            help='Generate table for specific test size only')
    parser_args.add_argument('--readme-style', action='store_true', 
                            help='Generate README-style summary table (tiny test only)')
    parser_args.add_argument('--sections', action='store_true',
                            help='Generate one file with sections for each test size')
    parser_args.add_argument('--all', action='store_true',
                            help='Generate all formats: sectioned file + complete comparison + README style')
    
    args = parser_args.parse_args()
    
    # Initialize parser
    parser = LogParser()
    
    print("=== Matrix Operations Log Parser ===")
    print("Parsing all log files...")
    all_data = parser.parse_all_logs()
    
    if not all_data:
        print("No log files found or parsed successfully.")
        return
    
    test_names = parser.get_test_names(all_data)
    print(f"Found {len(all_data)} languages and {len(test_names)} test configurations")
    
    if args.all:
        # Generate all formats
        print("\n=== Generating ALL formats ===")
        
        # 1. Generate one file with sections for each test size
        print("1. Generating sectioned file for all test sizes...")
        content = "# Matrix Operations Performance Comparison\n\n"
        content += "Comprehensive performance comparison across different languages and matrix sizes.\n\n"
        content += "## Languages Tested\n"
        for language in ['C++', 'NumPy', 'pandas', 'R df', 'R mat']:
            if language in all_data:
                content += f"- **{language}**\n"
        content += "\n---\n\n"
        
        # Add each test as a section
        for i, test_name in enumerate(test_names):
            content += parser.format_comparison_table(all_data, test_name)
            if i < len(test_names) - 1:  # Add separator between sections except for last
                content += "\n---\n\n"
        
        save_to_file(content, "matrix_performance_all_tests.md")
        
        # 2. Generate complete comparison
        print("2. Generating complete comparison...")
        content = "# Complete Matrix Operations Performance Comparison\n\n"
        content += "Performance comparison across different languages and matrix sizes.\n\n"
        content += "## Languages Tested\n"
        for language in ['C++', 'NumPy', 'pandas', 'R df', 'R mat']:
            if language in all_data:
                content += f"- **{language}**\n"
        content += "\n"
        
        for test_name in test_names:
            content += parser.format_comparison_table(all_data, test_name)
            content += "\n"
        
        save_to_file(content, "complete_performance_comparison.md")
        
        # 3. Generate README-style table
        print("3. Generating README-style summary...")
        tiny_test = None
        for test_name in test_names:
            if 'tiny' in test_name.lower():
                tiny_test = test_name
                break
        
        if tiny_test:
            content = "# Matrix Operations Performance Comparison\n\n"
            content += f"Performance comparison across different languages for {tiny_test}:\n\n"
            content += parser.format_summary_table(all_data, tiny_test)
            save_to_file(content, "README_performance_table.md")
        
        print("✅ All formats generated successfully!")
        return
    
    elif args.sections:
        # Generate one file with separate sections for each test size
        print("Generating one file with sections for each test size...")
        
        content = "# Matrix Operations Performance Comparison\n\n"
        content += "Comprehensive performance comparison across different languages and matrix sizes.\n\n"
        content += "## Languages Tested\n"
        for language in ['C++', 'NumPy', 'pandas', 'R df', 'R mat']:
            if language in all_data:
                content += f"- **{language}**\n"
        content += "\n---\n\n"
        
        # Add each test as a section
        for i, test_name in enumerate(test_names):
            content += parser.format_comparison_table(all_data, test_name)
            if i < len(test_names) - 1:  # Add separator between sections except for last
                content += "\n---\n\n"
        
        if args.save:
            save_to_file(content, "matrix_performance_all_tests.md")
        else:
            print(content)
    
    elif args.readme_style:
        # Generate README-style table for tiny test
        tiny_test = None
        for test_name in test_names:
            if 'tiny' in test_name.lower():
                tiny_test = test_name
                break
        
        if tiny_test:
            content = "# Matrix Operations Performance Comparison\n\n"
            content += f"Performance comparison across different languages for {tiny_test}:\n\n"
            content += parser.format_summary_table(all_data, tiny_test)
            
            if args.save:
                save_to_file(content, "README_performance_table.md")
            else:
                print(content)
        else:
            print("Tiny test not found for README-style table")
    
    elif args.test:
        # Generate table for specific test size
        target_test = None
        for test_name in test_names:
            if args.test.lower() in test_name.lower():
                target_test = test_name
                break
        
        if target_test:
            content = f"# {target_test} Performance Results\n\n"
            content += parser.format_comparison_table(all_data, target_test)
            
            if args.save:
                filename = f"{args.test}_test_results.md"
                save_to_file(content, filename)
            else:
                print(content)
        else:
            print(f"Test size '{args.test}' not found")
    
    else:
        # Default: show basic console output for exploration
        print(f"\n=== Found data for {len(all_data)} languages ===")
        for language in all_data.keys():
            print(f"- {language}")
        
        print(f"\n=== Found {len(test_names)} test configurations ===")
        for i, test_name in enumerate(test_names, 1):
            print(f"{i}. {test_name}")
        
        print("\n" + "="*80)
        print("COMPREHENSIVE COMPARISON TABLES")
        print("="*80)
        
        # Generate comparison tables for each test
        for test_name in test_names:
            print(f"\n{parser.format_comparison_table(all_data, test_name)}")
        
        # Generate README-style summary for tiny test
        tiny_test = None
        for test_name in test_names:
            if 'tiny' in test_name.lower():
                tiny_test = test_name
                break
        
        if tiny_test:
            print("="*80)
            print("README-STYLE SUMMARY TABLE (Tiny Test)")
            print("="*80)
            print(f"\n{parser.format_summary_table(all_data, tiny_test)}")

if __name__ == "__main__":
    main()


"""
=== USAGE EXAMPLES ===

# Generate one file with all test sections (RECOMMENDED)
python matrix_log_parser.py --sections --save
# Creates: matrix_performance_all_tests.md with separate sections for all test sizes

# Generate everything at once
python matrix_log_parser.py --all --save
# Creates:
#   - matrix_performance_all_tests.md (sectioned file)
#   - complete_performance_comparison.md (alternative format)
#   - README_performance_table.md (summary for documentation)

# Generate specific test size
python matrix_log_parser.py --test tiny --save
python matrix_log_parser.py --test small --save
python matrix_log_parser.py --test medium --save
python matrix_log_parser.py --test large --save

# Generate README-style table
python matrix_log_parser.py --readme-style --save

# View results in console (remove --save)
python matrix_log_parser.py --sections
python matrix_log_parser.py --test tiny


=== TABLE FORMAT ===

Clean format with Orientation column:

| Operation | Orientation | C++ | NumPy | pandas |
| --- | --- | --- | --- | --- |
| sum | row<br>col | 0.000001 ± 0.000001s<br>0.000024 ± 0.000002s | ... |
| mean | row<br>col | 0.000009 ± 0.000001s<br>0.000010 ± 0.000001s | ... |

Key Features:
- Orientation column shows "row<br>col" for each operation
- Each timing cell: timing_row<br>timing_col with mean ± std
- Row-major timing on top, column-major timing on bottom


=== OUTPUT FILES ===

matrix_performance_all_tests.md     # ONE file with all test sections (recommended)
complete_performance_comparison.md  # Alternative format
README_performance_table.md         # Summary for documentation
tiny_test_results.md                # Individual test size (if using --test)

"""

