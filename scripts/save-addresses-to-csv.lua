-- File handle for the output CSV file.
local csv = assert(io.open("addresses.csv", "a"))

-- Index used in the next HD derivation path.
local index = 0

--- Returns the next HD derivation path. [Required]
-- @return HD derivation path.
NextPath = function ()
    if index == 0 then
        print("Started deriving addresses ...")
        -- Write CSV header on first run.
        csv:write("Path,Address\n")
    end

    local path = string.format("m/44'/60'/0'/0/%d", index)
    index = index + 1
    return path
end

--- Called when a new address is derived. [Required]
-- @param path HD derivation path.
-- @param checksum_address Checksummed Ethereum address.
OnAddress = function (path, checksum_address)
    csv:write(path, ",0x", checksum_address, "\n")
    if index % 25 == 0 and index > 0 then
        print("Dervied " .. index .. " addresses")
        csv:flush()
    end
end

--- Called when the program exits. [Required]
OnExit = function ()
    csv:close()
    print("Total " .. index .. " addresses derived")
end
