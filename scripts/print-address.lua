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
    print(path, "0x" .. checksum_address)
end

--- Called when the program exits. [Required]
OnExit = function ()
end
