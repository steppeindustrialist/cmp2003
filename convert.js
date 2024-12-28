const fs = require("fs");

function convertToHackerRankFormat() {
    try {
        const ratingsData = fs.readFileSync('./ratings.txt', 'utf8');
        const predictionsData = fs.readFileSync('./predictions.txt', 'utf8');
        
        const ratingsLines = ratingsData.split('\n').filter(line => line.trim());
        let output = 'train dataset\n';
        
        ratingsLines.forEach(line => {
            const parts = line.split(' ').filter(part => part.trim());
            if (parts.length >= 3) {
                const userId = parts[0];
                const itemId = parts[1];
                const rating = parseFloat(parts[2]).toFixed(1); 
                output += `${userId} ${itemId} ${rating}\n`;
            }
        });
        
        output += 'test dataset\n';
        const predictionsLines = predictionsData.split('\n').filter(line => line.trim());
        
        predictionsLines.forEach(line => {
            const parts = line.split(' ').filter(part => part.trim());
            if (parts.length >= 2) {
                const userId = parts[0];
                const itemId = parts[1];
                output += `${userId} ${itemId}\n`;
            }
        });
        
        fs.writeFileSync('combined_dataset.txt', output);
        
        const writtenData = fs.readFileSync('combined_dataset.txt', 'utf8');
        const lines = writtenData.split('\n');
        
        console.log('Conversion completed successfully!');
        console.log('Statistics:');
        console.log(`- Total lines written: ${lines.length}`);
        console.log(`- File size: ${fs.statSync('combined_dataset.txt').size} bytes`);
        
        let inTestSection = false;
        let trainCount = 0;
        let testCount = 0;
        
        lines.forEach(line => {
            if (line === 'train dataset') {
                inTestSection = false;
            } else if (line === 'test dataset') {
                inTestSection = true;
            } else if (line.trim()) {
                const parts = line.split(' ');
                if (!inTestSection) {
                    if (parts.length === 3) trainCount++;
                } else {
                    if (parts.length === 2) testCount++;
                }
            }
        });
        
        console.log(`- Training examples: ${trainCount}`);
        console.log(`- Test examples: ${testCount}`);
        
    } catch (error) {
        console.error('Error during conversion:');
        console.error(`- Error message: ${error.message}`);
        console.error(`- Stack trace: ${error.stack}`);
        process.exit(1);
    }
}

convertToHackerRankFormat();