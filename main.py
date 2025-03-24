import os

def merge_text_files(directory):
    # Open the output file in write mode
    with open('data/data.txt', 'w') as output_file:
        # Iterate over all files in the given directory
        for filename in os.listdir(directory):
            if filename.endswith('.txt'):
                file_path = os.path.join(directory, filename)
                
                with open(file_path, 'r') as input_file:
                    # Write <start> and the contents of the file to the output file
                    #output_file.write('<start>\n')
                    output_file.write(input_file.read())
                    # Write <end> after the content
                    #output_file.write('<end>\n')


def main():
    directory = 'screenplays'
    merge_text_files(directory)

if __name__ == '__main__':
    main()
