#!/bin/sh

# get input arguments
while getopts "i:o:hp" opt; do
  case $opt in
    i)
      input_folder=$OPTARG
      ;;
    o)
      output_folder=$OPTARG
      ;;
    h)
        echo "Usage: HybridBatch.sh -i <input_folder> -o <output_folder> -p"
        echo -e "\t -i: input folder, contains all the input y4m files."
        echo -e "\t -o: output folder, folder where each test will output it's results."
        echo -e "\t -p: decode file, if set, will decode and produce y4m files."
        exit 0
        ;;
    p)
      decodefile=1
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      ;;
  esac
done

# check if input_folder and output_folder are set
if [ -z "$input_folder" ]; then
  echo "Error: -i option is required." >&2
  exit 1
fi

if [ -z "$output_folder" ]; then
  echo "Error: -o option is required" >&2
  exit 1
fi

# check that executables exists in build/Deliverable_6 folder
executable_path="./build/Deliverable_6"

if [ ! -f "$executable_path/HybridEncoderExample.out" ]; then
    echo "Error: HybridEncoderExample.out not found in $executable_path" >&2
    exit 1
fi

if [ ! -f "$executable_path/HybridDecoderExample.out" ]; then
    echo "Error: HybridDecoderExample.out not found in $executable_path" >&2
    exit 1
fi

# get list of files in input_folder
files=$(ls $input_folder)

echo "Input folder: $input_folder"

# loop and print each filename
for file in $files
do
    echo -e " - $file"
done

numberofinputs=$(ls $input_folder | wc -l)
sizeofinput=$(du -sh $input_folder | cut -f1)
sizeofoneinput=$(echo "scale=2; $sizeofinput/$numberofinputs" | bc)

avgencodedfile=0.8

numberoffiles=$((5*2*2*2*$(ls $input_folder | wc -l)))

encodedspace=$(echo "scale=2; $numberoffiles*$avgencodedfile" | bc)
decodedfilespace=$(echo "scale=2; $numberoffiles*$sizeofoneinput" | bc)

totalspace=$(echo "scale=2; $encodedspace+$decodedfilespace" | bc)

echo "These are the files that are going to be encoded."
if [ "$decodefile" == 1 ]; then
    echo "The files will also be decoded."
    echo "This will produce a lot of files, roughly $(($(echo $numberoffiles)*2)) files."
    echo "Since the files are decoded, you will need roughly $totalspace gb of free space."
else
    echo "The files will not be decoded."
    echo "This will produce a lot of files, roughly $numberoffiles files."
    echo "Since the files are not decoded, you will need roughly $encodedspace gb of free space."
    echo "If you want to decode the files, use the -p option."
fi
echo "Do you want to continue? (y/n)"
read answer

if [ "$answer" != "${answer#[Yy]}" ] ;then
    echo "Starting encoding..."
else
    echo "Exiting..."
    exit 0
fi

# create output folder if it doesn't exist
if [ ! -d "$output_folder" ]; then
  mkdir $output_folder
fi

# loop and encode each file
for file in $files
do
    # create folder for each file
    folder_name="${file%.*}"
    mkdir $output_folder/$folder_name
    sizeOfFile=$(stat -c%s "$input_folder/$file")
    # create file to write results
    echo "File: $file" > $output_folder/$folder_name/results.txt
    # loop through 0 to 7
    for i in {3..7} # predictors
    do
        for r in {0..1} # period
        do
            for j in {1..2} #block range
            do
                for n in {0..1} # shift
                do
                    temp_name="${i}_$(($j*5))_$(($n*4))_$(($r*15))"
                    if [ -d "$output_folder/$folder_name/$temp_name" ]; then
                        echo "Folder $output_folder/$folder_name/$temp_name already exists. Skipping..."
                        continue
                    fi
                    mkdir $output_folder/$folder_name/$temp_name
                    # if folder exists, continue
                    filename_withoutextension="${file%.*}"
                    outputfilename="${filename_withoutextension}_$temp_name.bin"
                    outputfilename2="${filename_withoutextension}_decoded_$temp_name.y4m"
                    # if r == 0 dont pass the flag
                    if [ $r -eq 0 ]; then
                        encondingtime=$(./build/Deliverable_6/HybridEncoderExample.out -i $input_folder/$file -o $output_folder/$folder_name/$temp_name/$outputfilename -p $i -b $(($j*5)) -s $(($n*4)))
                    else
                        encondingtime=$(./build/Deliverable_6/HybridEncoderExample.out -i $input_folder/$file -o $output_folder/$folder_name/$temp_name/$outputfilename -p $i -b $(($j*5)) -s $(($n*4)) -r $(($r*15)))
                    fi
        
                    # decode file
                    if [ "$decodefile" == 1 ]; then
                        decodingtime=$(./build/Deliverable_6/HybridDecoderExample.out -i $output_folder/$folder_name/$temp_name/$outputfilename -o $output_folder/$folder_name/$temp_name/$outputfilename2)
                    else
                        decodingtime=N/A
                    fi

                    # calculate compression ratio
                    compressedsize=$(stat -c%s "$output_folder/$folder_name/$temp_name/$outputfilename")
                    compressionratio=$(echo "scale=2; $sizeOfFile/$compressedsize" | bc)
                    compressedratio2=$(echo "scale=2; $compressedsize/$sizeOfFile*100" | bc)
                    # write results to file
                    echo "$i & $(($j*5)) & $(($n*4)) & $((r*15)) & $encondingtime & $decodingtime & $compressedsize & $compressionratio && $compressedratio2 % \\\\" >> $output_folder/$folder_name/results.txt
                    echo "\\hline" >> $output_folder/$folder_name/results.txt
                done
            done
        done
    done    
done
