// Copyright 2015 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TRUNKS_POLICY_SESSION_H_
#define TRUNKS_POLICY_SESSION_H_

#include <string>
#include <vector>

#include <base/macros.h>

#include "trunks/tpm_generated.h"

namespace trunks {

class AuthorizationDelegate;

// PolicySession is an interface for managing policy backed sessions for
// authorization and parameter encryption.
class PolicySession {
 public:
  PolicySession() {}
  virtual ~PolicySession() {}

  // Returns an authorization delegate for this session. Ownership of the
  // delegate pointer is retained by the session.
  virtual AuthorizationDelegate* GetDelegate() = 0;

  // Starts a salted session which is bound to |bind_entity| with
  // |bind_authorization_value|. Encryption is enabled if |enable_encryption| is
  // true. The session remains active until this object is destroyed or another
  // session is started with a call to Start*Session.
  virtual TPM_RC StartBoundSession(
      TPMI_DH_ENTITY bind_entity,
      const std::string& bind_authorization_value,
      bool enable_encryption) = 0;

  // Starts a salted, unbound session. Encryption is enabled if
  // |enable_encryption| is true. The session remains active until this object
  // is destroyed or another session is started with a call to Start*Session.
  virtual TPM_RC StartUnboundSession(bool enable_encryption) = 0;

  // This method is used to get the current PolicyDigest of the PolicySession.
  virtual TPM_RC GetDigest(std::string* digest) = 0;

  // This method is used to construct a complex policy. It takes a list
  // of policy digests. After the command is executed, the policy represented
  // by this session is the OR of the provided policies.
  virtual TPM_RC PolicyOR(const std::vector<std::string>& digests) = 0;

  // This method binds the PolicySession to a provided PCR value.
  virtual TPM_RC PolicyPCR(uint32_t pcr_index,
                           const std::string& pcr_value) = 0;

  // This method binds the PolicySession to a specified CommandCode.
  // Once called, this Session can only be used to authorize actions on the
  // provided CommandCode.
  virtual TPM_RC PolicyCommandCode(TPM_CC command_code) = 0;

  // This method specifies that Authorization Values need to be included in
  // HMAC computation done by the AuthorizationDelegate.
  virtual TPM_RC PolicyAuthValue() = 0;

  // Sets the current entity authorization value. This can be safely called
  // while the session is active and subsequent commands will use the value.
  virtual void SetEntityAuthorizationValue(const std::string& value) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(PolicySession);
};

}  // namespace trunks

#endif  // TRUNKS_POLICY_SESSION_H_
