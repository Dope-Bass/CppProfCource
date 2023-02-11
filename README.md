# Options

### -h [ --help ]                
  - Show options list

### -i [ --include_dir ] arg     
  - Set directory for scan. Current dir by default
  
### -e [ --exclude_dir ] arg     
  - Exclude directory from scan
  
### -l [ --level ] arg (=0)      
  - Level of scanning:                
                1 - with nested dirs                
                0 - without nested dirs
                               
### -s [ --size ] arg (=1)       
  - Minimal size of file to scan - default > 1 byte
  
### -m [ --mask ] arg            
  - File masks to scan - ex: "*.txt"
  
### -b [ --block_size ] arg (=1) 
  - Size of block using for file reading
  
### -H [ --hash ] arg (=crc32)   
  - Hash algorithm - crc32 or FNV
