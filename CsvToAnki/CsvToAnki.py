import csv
import os

path = os.path.join(os.getcwd(), "csvFiles")
open("ankiFile.txt", 'w').close() # Erase ankiFile.txt before appending new flashcards

for filename in os.listdir(path):
   with open(os.path.join(path, filename), 'r', encoding="utf8") as csvFile:
      reader = csv.DictReader(csvFile)
      for row in reader:
         for key in reader.fieldnames[1:]:
            with open("ankiFile.txt", 'a', encoding="utf8") as ankiFile:
                prompt = row[reader.fieldnames[0]] + ": " + key
                answer = row[key]
                writer = csv.writer(ankiFile)
                writer.writerow([prompt, answer])