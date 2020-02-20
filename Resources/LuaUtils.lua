function dump(o)
    if type(o) == 'table' then
        local s = '{'
        for k,v in pairs(o) do
            if type(k) ~= 'number' then
                s = s .. k .. '=' .. dump(v) .. ','
            else
                s = s .. dump(v) .. ','
            end
        end
        return s .. '}'
    elseif type(o) == 'string' then
        return '"' .. tostring(o) .. '"'
    else
        return tostring(o)
    end
end
