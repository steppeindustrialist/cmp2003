const fs = require("fs")

function convertRatings() {
    const data = fs.readFileSync('./ratings.txt', 'utf8');
    
    const lines = data.split('\n').filter(line => line.trim());
    
    // ratings.txt => public_training_data.txt
    let output = 'UserID,ItemID,Rating\n';
    
    lines.forEach(line => {
        const parts = line.split(' ').filter(part => part.trim());
        
        if (parts.length >= 3) {
            output += `${parts[0]},${parts[1]},${parts[2]}\n`;
        }
    });
    
    fs.writeFileSync('ratings.csv', output);

    console.log('Created ratings.csv');
}

function convertPredictions() {
    const data = fs.readFileSync('./predictions.txt', 'utf8');
    
    const lines = data.split('\n').filter(line => line.trim());
   
    // predictions.txt => public_test_data.txt 
    let output = 'UserID,ItemID\n';
    
    lines.forEach((line) => {
        const parts = line.split(' ').filter(part => part.trim());
        
        if (parts.length >= 2) {
            output += `${parts[0]},${parts[1]}\n`;
        }
    });
    
    fs.writeFileSync('predictions.csv', output);
    
    console.log('Created predictions.csv');
}

try {
    convertRatings();
    convertPredictions();
    console.log('Conversion completed successfully!');
} catch (error) {
    console.error('Error during conversion:', error.message);
}