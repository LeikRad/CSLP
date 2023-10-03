    // int M;
    // std::vector<u_char> buffer;

void BitStream::WriteBit()
{
    u_char byte = buffer[0];
    u_char q = byte / M;
    u_char r = byte % M;

    for (int i = 0; i < q; i++)
    {
        out << 1;
    }

    out << 0;

    u_char b = log2(M);

    if (r < pow(2, b + 1))
    {
        for (int i = b - 1; i >= 0; i--)
        {
            out << (r >> i) % 2;
        }
    }
    else
    {
        r += pow(2, b + 1) - M;
        for (int i = b; i >= 0; i--)
        {
            out << (r >> i) % 2;
        }
    }
};