-- Useful functions for finding vanity addresses.
local vanity = require("vanity")

-- Index used in the next HD derivation path.
local index = 0

--- Returns the next HD derivation path. [Required]
-- @return HD derivation path.
NextPath = function ()
    local path = string.format("m/44'/60'/0'/0/%d", index)
    index = index + 1
    return path
end

--- Called when a new address is derived. [Required]
-- @param path HD derivation path.
-- @param checksum_address Checksummed Ethereum address.
OnAddress = function (path, checksum_address)
    -- Case insensitive matching.
    local address = string.lower(checksum_address)
    -- Require either 3 leading or 4 repeating characters anywhere to be considered vanity address.
    -- TODO: Implement custom vanity address matching logic here.
    local is_vanity = vanity.has_leading_repeating_characters(address, 3)
        or vanity.has_repeating_characters(address, 4)

    print(path, "0x" .. checksum_address, is_vanity and "Vanity" or "")
end

--- Called when the program exits. [Required]
OnExit = function ()
    print("Done")
end
