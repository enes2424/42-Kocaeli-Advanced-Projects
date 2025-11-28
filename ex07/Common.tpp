template <typename V>
V lerp(V u, V v, float t)
{
	if (t == 0.f)
		return u;

	if (t == 1.f)
		return v;

	if (t < 0.f || t > 1.f)
		throw std::out_of_range("lerp: t must be in [0, 1] range");

	return u * (1.f - t) + v * t;
}
