#pragma once
#ifndef OVERMAP_CONNECTION_H
#define OVERMAP_CONNECTION_H

#include "enums.h"
#include "int_id.h"
#include "omdata.h"
#include "string_id.h"

#include <list>
#include <vector>

class JsonObject;
class JsonIn;

class overmap_connection
{
    public:
        class subtype
        {
                friend overmap_connection;

            public:
                enum class flag { orthogonal };

            public:
                string_id<oter_type_t> terrain;

                int basic_cost = 0;

                bool allows_terrain( const int_id<oter_t> &oter ) const;
                bool allows_turns() const {
                    return terrain->is_linear();
                }

                bool is_orthogonal() const {
                    return flags.count( flag::orthogonal );
                }

                void load( JsonObject &jo );
                void deserialize( JsonIn &jsin );

            private:
                std::set<string_id<overmap_location>> locations;
                std::set<flag> flags;
        };

    public:
        const subtype *pick_subtype_for( const int_id<oter_t> &ground ) const;
        bool has( const int_id<oter_t> &oter ) const;

        void load( JsonObject &jo, const std::string &src );
        void check() const;
        void finalize();

    public:
        string_id<overmap_connection> id;
        bool was_loaded = false;

    private:
        struct cache {
            const subtype *value = nullptr;
            bool assigned = false;
            operator bool() const {
                return assigned;
            }
        };

        std::list<subtype> subtypes;
        mutable std::vector<cache> cached_subtypes;
};

namespace overmap_connections
{

void load( JsonObject &jo, const std::string &src );
void finalize();
void check_consistency();
void reset();

string_id<overmap_connection> guess_for( const int_id<oter_type_t> &oter_id );
string_id<overmap_connection> guess_for( const int_id<oter_t> &oter_id );

}

#endif // OVERMAP_CONNECTION_H