/*
 * File:        hashmap.c
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-13
 * Last edit:   2021-05-13
 * Description: Function definitions to create, access and destroy hashmap instaces
 */


#define __HASHMAP_C
#include <hashmap.h>

 
/// Create a new hashmap 
void newHashmap (
    Hashmap *p_hashmap,
    size_t elem_c
) {
    p_hashmap->map_cap = elem_c;
    p_hashmap->used_size = 0;
    p_hashmap->map_data = (__HashData*) calloc (
        p_hashmap->map_cap,
        sizeof(__HashData)
    );

    p_hashmap->indices = (size_t*) calloc (
        p_hashmap->map_cap,
        sizeof(size_t)
    );
}


/// Hashing function for calculating the element index
/// This function is based on mostly Jenkins one at time hashing algorihm
/// Steps to finding index are following:
/// 1. Find crc32_key from key data
/// 2. Perform Jenkins one at time bitwise operations
/// 3. Perform three other Jenkins operations
/// 4. Multiply bit-shifted out_key with constant 0x9E3779B1
/// 5. Return hash % n
static size_t __hashfunc (
    void *key,
    size_t key_size,
    size_t map_c
) {
    size_t i;

    // Calculate the crc32 key form given key bytes
    size_t crc32_key = 0;
    for(i = 0; i < key_size; i++)
        crc32_key = crc32_table[(crc32_key ^ ((uint8_t*) key)[i]) & 0xff] ^ (crc32_key >> 8);

    // Jenkins one at time key calculation
    size_t out_key = 0;
    for(i = 0;  i < key_size; i++) {
        out_key += (uint32_t) crc32_key;
        out_key += out_key << 10;
        out_key ^= out_key >> 6;
    }
    out_key += out_key << 3;
    out_key ^= out_key >> 11;
    out_key += out_key << 15;

    // Divide the result by 32 and multiply with prime constant
    out_key *= (out_key >> 5) * 0x9E3779B1;
    return out_key % map_c;
}


/// Reallocate twice the capacity for buckets in hashmap 
static void __reallocateHashmap(Hashmap *p_hm) {
    size_t old_size = p_hm->map_cap;
    p_hm->map_cap <<= 2;
    __HashData *tmp = (__HashData*) calloc(p_hm->map_cap, sizeof(__HashData));

    // Copy data from old map to a new one
    size_t i = 0, j = 0;
    for(i = 0; i < old_size; i++) {
        // Check if the key exists for current entry
        if(p_hm->map_data[i].key) {
            // Generate new memory location index from hashing function
            j = __hashfunc(p_hm->map_data[i].key, p_hm->map_data[i].key_len,
                p_hm->map_cap);
            
                        
            // Check if the index collides with an already existing entry
            if((tmp[j].data || tmp[j].key) && __keycmp(p_hm->map_data[i].key,
               p_hm->map_data[i].key_len, tmp[j].key, p_hm->map_data[i].key_len)) {
                bool is_found = false;
                int64_t i = 0;

                // Check for empty array spaces after collided key value 
                for(i = (int64_t) j; i < (int64_t) p_hm->map_cap; i++) {
                    if(!tmp[i].key && !tmp[i].data) {
                        is_found = true;
                        break;
                    }
                }
                
                // Check for empty array spaces before collided key value
                if(!is_found) {
                    for(i = (int64_t) j; i >= 0; i--) {
                        if(!tmp[i].key && !tmp[i].data) {
                            is_found = true;
                            break;
                        }
                    }
                }

                tmp[i].key = p_hm->map_data[i].key;
                tmp[i].key_len = p_hm->map_data[i].key_len;
                tmp[i].data = p_hm->map_data[i].data;
            }
           
            else {
                tmp[j].key = p_hm->map_data[i].key;
                tmp[j].key_len = p_hm->map_data[i].key_len;
                tmp[j].data = p_hm->map_data[i].data;
            }
        }
    }

    free(p_hm->map_data);
    p_hm->map_data = tmp;
}


/*
 * Key comparisson method
 * Returns 0 if keys are equal, 1 if key1 is longer than key2, -1 if 
 * key2 is longer than key1 then returns 2 if keys are the same length
 * but their memory areas do not match
 */
static int __keycmp(void *key1, size_t n1, void *key2, size_t n2) {
    if(n1 < n2) return -1;
    else if(n1 > n2) return 1;
    else if(!memcmp(key1, key2, n1))
        return 0;
    else return 2;
}


/*
 * Search for the bucket index of the key element
 */
static int64_t __findIndex(Hashmap *p_hm, void *key, size_t key_size) {
    // INT64_MAX is the default value to return when no item with given key was found
    int64_t out = INT64_MAX;
    
    // Calculate the element index according to hashing function
    int64_t index = (int64_t) __hashfunc (
        key, key_size, p_hm->map_cap);

    // Check if no key collision has happened
    if(!__keycmp(p_hm->map_data[index].key, p_hm->map_data[index].key_len, key, key_size))
        out = (int64_t) index;
    
    // Key collision has happened, so perform linear search for the correct element
    else {
        // Perform linear search for the key
        int64_t i = 0;
        bool is_found = false;

        // For each element after the current one in the bucket, check its key
        for(i = index; i < (int64_t) p_hm->map_cap; i++) {
            if(p_hm->map_data[i].key && p_hm->map_data[i].key_len && !__keycmp(p_hm->map_data[i].key, 
               p_hm->map_data[i].key_len, key, key_size)) {
                out = (uint64_t) i;
                is_found = true;
                break;
            }
        }

        // If correct location was not found after the previous element instance 
        // search from the front of the array
        if(!is_found) {
            // For each element before the current on in the bucker, check its key
            for(i = index; i >= 0; i--) {
                if(p_hm->map_data[i].key && p_hm->map_data[i].key_len && !__keycmp(p_hm->map_data[i].key, 
                   p_hm->map_data[i].key_len, key, key_size)) {
                    out = (int64_t) i;
                    is_found = true;
                    break;
                }
            }
        }
    }
    
    return out;
}


/// Push value and it's key to hashmap 
void pushToHashmap (
    Hashmap *p_hm,
    void *key,
    size_t key_size,
    void *data
) {
    // Check if any bucket reallocation is needed
    if(p_hm->used_size + 1 >= p_hm->map_cap)
       __reallocateHashmap(p_hm); 

    // Check if value with current key already exits
    size_t index = __hashfunc (
        key, key_size, p_hm->map_cap);

    // If the calculated index is not available then search for the nearest free memory space
    if((p_hm->map_data[index].data || p_hm->map_data[index].key) && 
       __keycmp(p_hm->map_data[index].key, p_hm->map_data[index].key_len, key, key_size)) {
        bool is_found = false;
        int64_t i = 0;

        // Check for empty array spaces after the occupied memory area
        for(i = (int64_t) index; i < (int64_t) p_hm->map_cap; i++) {
            if(!p_hm->map_data[i].key && !p_hm->map_data[i].data) {
                is_found = true;
                break;
            }
        }
        
        // Check for empty array spaces before collided memory area 
        if(!is_found) {
            for(i = (int64_t) index; i >= 0; i--) {
                if(!p_hm->map_data[i].key && !p_hm->map_data[i].data) {
                    is_found = true;
                    break;
                }
            }
        }

        // Set the key and value pairs in array
        p_hm->map_data[i].key = key;
        p_hm->map_data[i].key_len = key_size;
        p_hm->map_data[i].data = data;
        p_hm->indices[p_hm->used_size] = i; 
    }
   
    // Index is available
    else {
        p_hm->map_data[index].data = data;
        p_hm->map_data[index].key_len = key_size;
        p_hm->map_data[index].key = key;
        p_hm->indices[p_hm->used_size] = index; 
    }

    // Increment the used map size
    p_hm->used_size++;
}



/// Find value with certain key 
void *findValue (
    Hashmap *p_hm,
    void *key,
    size_t key_len
) {
    int64_t i = __findIndex(p_hm, key, key_len);
    if(i == INT64_MAX) return NULL;
    return p_hm->map_data[i].data;
}


/// Pop the value that is specified with the key from hashmap 
void *popFromHashmap (
    Hashmap *p_hm,
    void *key,
    size_t key_size
) {
    // Find the index to pop
    int64_t ind = __findIndex(p_hm, key, key_size);

    // Nothing to pop
    if(ind == INT64_MAX) return NULL;

    // Set assign the output data pointer according to the index
    void *data = p_hm->map_data[ind].data;
    p_hm->map_data[ind].data = NULL;
    p_hm->map_data[ind].key = NULL;
    p_hm->map_data[ind].key_len = 0;

    // Shift the indices array values, since the previous element was removed
    bool shift = false;
    for(size_t i = 0; i < p_hm->used_size; i++) {
        if(!shift && p_hm->indices[i] == ind)
            shift = true;
        
        else if(shift) {
            p_hm->indices[i - 1] = p_hm->indices[i];
            if(i == p_hm->used_size - 1)
                p_hm->indices[i] = 0;
        }
    }

    p_hm->used_size--;
    return data;
}


/// Clear all values that are in the hashmap
void clearHashmap(Hashmap *p_hm) {
    memset(p_hm->indices, 0, sizeof(size_t) * p_hm->map_cap);
    memset(p_hm->map_data, 0, sizeof(__HashData) * p_hm->map_cap);
}


/// Destroy the given hashmap instance
void destroyHashmap(Hashmap *p_hm) {
    free(p_hm->map_data);
    free(p_hm->indices);
}
