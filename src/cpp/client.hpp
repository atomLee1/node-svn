#pragma once

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <unordered_map>
#include <vector>

#include <cpp/types.hpp>

struct apr_pool_t;
struct svn_client_ctx_t;

namespace svn {
class client : public std::enable_shared_from_this<client> {
  public:
    using simple_auth_provider = std::shared_ptr<std::function<std::optional<simple_auth>(const std::string&,
                                                                                          const std::optional<const std::string>&,
                                                                                          bool)>>;

    using get_changelists_callback = std::function<void(const char*, const char*)>;
    using cat_callback             = std::function<void(const char*, size_t)>;
    using commit_callback          = std::function<void(const commit_info*)>;
    using info_callback            = std::function<void(const char*, const svn::info&)>;
    using remove_callback          = std::function<void(const commit_info*)>;
    using status_callback          = std::function<void(const char*, const svn::status&)>;
    using notify_function          = std::shared_ptr<std::function<void(const notify_info&)>>;

    using blame_callback = std::function<void(int32_t                start_revision,
                                              int32_t                end_revision,
                                              int64_t                line_number,
                                              std::optional<int32_t> revision,
                                              std::optional<int32_t> merged_revision,
                                              const char*            merged_path,
                                              const char*            line,
                                              bool                   local_change)>;

    explicit client(const std::optional<const std::string>& config_path);
    client(client&&);
    client(const client&) = delete;

    client& operator=(client&&);
    client& operator=(const client&) = delete;

    ~client();

    void add_notify_function(std::initializer_list<notify_action> actions, const notify_function function);
    void remove_notify_function(std::initializer_list<notify_action> actions, const notify_function function);
    void invoke_notify_function(const notify_info& info);

    void                       add_simple_auth_provider(const simple_auth_provider provider);
    void                       remove_simple_auth_provider(const simple_auth_provider provider);
    std::optional<simple_auth> invoke_simple_auth_providers(const std::string&                      realm,
                                                            const std::optional<const std::string>& username,
                                                            bool                                    may_save);

    void add_to_changelist(const std::string&   path,
                           const std::string&   changelist,
                           depth                depth       = depth::infinity,
                           const string_vector& changelists = string_vector()) const;
    void add_to_changelist(const string_vector& paths,
                           const std::string&   changelist,
                           depth                depth       = depth::infinity,
                           const string_vector& changelists = string_vector()) const;

    void get_changelists(const std::string&              path,
                         const get_changelists_callback& callback,
                         depth                           depth       = depth::infinity,
                         const string_vector&            changelists = string_vector()) const;

    void remove_from_changelists(const std::string&   path,
                                 depth                depth       = depth::infinity,
                                 const string_vector& changelists = string_vector()) const;
    void remove_from_changelists(const string_vector& paths,
                                 depth                depth       = depth::infinity,
                                 const string_vector& changelists = string_vector()) const;

    void add(const std::string& path,
             depth              depth        = depth::infinity,
             bool               force        = true,
             bool               no_ignore    = false,
             bool               no_autoprops = false,
             bool               add_parents  = true) const;

    void blame(const std::string&    path,
               const revision&       start_revision,
               const revision&       end_revision,
               const blame_callback& callback,
               const revision&       peg_revision             = revision_kind::working,
               diff_ignore_space     ignore_space             = diff_ignore_space::none,
               bool                  ignore_eol_style         = true,
               bool                  ignore_mime_type         = false,
               bool                  include_merged_revisions = false) const;

    string_map cat(const std::string&  path,
                   const cat_callback& callback,
                   const revision&     peg_revision    = revision(revision_kind::working),
                   const revision&     op_revision     = revision(revision_kind::working),
                   bool                expand_keywords = true) const;
    cat_result cat(const std::string& path,
                   const revision&    peg_revision    = revision(revision_kind::working),
                   const revision&    op_revision     = revision(revision_kind::working),
                   bool               expand_keywords = true) const;

    int32_t checkout(const std::string& url,
                     const std::string& path,
                     const revision&    peg_revision             = revision(revision_kind::working),
                     const revision&    op_revision              = revision(revision_kind::working),
                     depth              depth                    = depth::infinity,
                     bool               ignore_externals         = false,
                     bool               allow_unver_obstructions = false) const;

    void cleanup(const std::string& path,
                 bool               break_locks,
                 bool               fix_recorded_timestamps,
                 bool               clear_dav_cache,
                 bool               vacuum_pristines,
                 bool               include_externals) const;

    void commit(const std::string&     path,
                const std::string&     message,
                const commit_callback& callback,
                depth                  depth                  = depth::infinity,
                const string_vector&   changelists            = string_vector(),
                const string_map&      revprop_table          = string_map(),
                bool                   keep_locks             = true,
                bool                   keep_changelists       = false,
                bool                   commit_as_operations   = false,
                bool                   include_file_externals = true,
                bool                   include_dir_externals  = true) const;
    void commit(const string_vector&   paths,
                const std::string&     message,
                const commit_callback& callback,
                depth                  depth                  = depth::infinity,
                const string_vector&   changelists            = string_vector(),
                const string_map&      revprop_table          = string_map(),
                bool                   keep_locks             = true,
                bool                   keep_changelists       = false,
                bool                   commit_as_operations   = false,
                bool                   include_file_externals = true,
                bool                   include_dir_externals  = true) const;

    void info(const std::string&   path,
              const info_callback& callback,
              const revision&      peg_revision      = revision_kind::unspecified,
              const revision&      op_revision       = revision_kind::unspecified,
              depth                depth             = depth::empty,
              bool                 fetch_excluded    = true,
              bool                 fetch_actual_only = true,
              bool                 include_externals = false,
              const string_vector& changelists       = string_vector()) const;

    void remove(const std::string&     path,
                const remove_callback& callback,
                bool                   force         = true,
                bool                   keep_local    = false,
                const string_map&      revprop_table = string_map()) const;
    void remove(const string_vector&   paths,
                const remove_callback& callback,
                bool                   force         = true,
                bool                   keep_local    = false,
                const string_map&      revprop_table = string_map()) const;

    void resolve(const std::string& path,
                 depth              depth  = depth::empty,
                 conflict_choose    choose = conflict_choose::merged) const;

    void revert(const std::string&   path,
                depth                depth             = depth::infinity,
                const string_vector& changelists       = string_vector(),
                bool                 clear_changelists = true,
                bool                 metadata_only     = false,
                bool                 added_keep_local  = false) const;
    void revert(const string_vector& paths,
                depth                depth             = depth::infinity,
                const string_vector& changelists       = string_vector(),
                bool                 clear_changelists = true,
                bool                 metadata_only     = false,
                bool                 added_keep_local  = false) const;

    int32_t status(const std::string&     path,
                   const status_callback& callback,
                   const revision&        op_revision        = revision(revision_kind::working),
                   depth                  depth              = depth::infinity,
                   bool                   get_all            = false,
                   bool                   check_out_of_date  = false,
                   bool                   check_working_copy = true,
                   bool                   no_ignore          = false,
                   bool                   ignore_externals   = false,
                   bool                   depth_as_sticky    = false,
                   const string_vector&   changelists        = string_vector()) const;

    int32_t              update(const std::string& path,
                                const revision&    op_revision              = revision(revision_kind::head),
                                depth              depth                    = depth::infinity,
                                bool               depth_is_sticky          = false,
                                bool               ignore_externals         = false,
                                bool               allow_unver_obstructions = false,
                                bool               adds_as_modification     = false,
                                bool               make_parents             = true) const;
    std::vector<int32_t> update(const string_vector& paths,
                                const revision&      op_revision              = revision(revision_kind::head),
                                depth                depth                    = depth::infinity,
                                bool                 depth_is_sticky          = false,
                                bool                 ignore_externals         = false,
                                bool                 allow_unver_obstructions = false,
                                bool                 adds_as_modification     = false,
                                bool                 make_parents             = true) const;

    std::string get_working_copy_root(const std::string& path) const;

  private:
    apr_pool_t*       _pool;
    svn_client_ctx_t* _context;

    std::unordered_map<svn::notify_action, std::set<notify_function>> _notify_functions;
    std::set<simple_auth_provider>                                    _simple_auth_providers;
};
} // namespace svn
