#pragma once

template <class Resource>
class ScopedResource {
public:
    using Deleter = std::function<void(Resource)>;
    ScopedResource(Resource resource, const Deleter& deleter)
        : m_resource(resource)
        , m_deleter(deleter)
    {
    }

    auto get() const
    {
        return m_resource;
    }

    ~ScopedResource()
    {
        if (m_resource) {
            Deleter(m_resource);
        }
    }

private:
    Resource m_resource;
    Deleter m_deleter;
};