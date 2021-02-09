// SPDX-License-Identifier: Apache-2.0
//===-- ssvm/api/ssvm.h - SSVM C API --------------------------------------===//
//
// Part of the SSVM Project.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains the function declarations of SSVM C API.
///
//===----------------------------------------------------------------------===//

#ifndef __SSVM_C_API_H__
#define __SSVM_C_API_H__

#if defined(_WIN32)
#ifdef SSVM_COMPILE_LIBRARY
#define SSVM_CAPI_EXPORT __declspec(dllexport)
#else
#define SSVM_CAPI_EXPORT __declspec(dllimport)
#endif /// SSVM_COMPILE_LIBRARY
#else
#define SSVM_CAPI_EXPORT __attribute__((visibility("default")))
#endif /// _WIN32

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/// SSVM version.
#define SSVM_VERSION "0.7.3"
#define SSVM_VERSION_MAJOR 0
#define SSVM_VERSION_MINOR 7
#define SSVM_VERSION_PATCH 3

/// Opaque struct of WASM execution result.
typedef struct {
} SSVM_Result;

/// WASM Proposal enumeration.
enum SSVM_Proposal
#ifdef __cplusplus
    : uint8_t
#endif // __cplusplus
{ SSVM_Proposal_Annotations = 0,
  SSVM_Proposal_BulkMemoryOperations,
  SSVM_Proposal_ExceptionHandling,
  SSVM_Proposal_FunctionReferences,
  SSVM_Proposal_Memory64,
  SSVM_Proposal_ReferenceTypes,
  SSVM_Proposal_SIMD,
  SSVM_Proposal_TailCall,
  SSVM_Proposal_Threads,
  SSVM_Proposal_Max };

/// Host Module Registration enumeration.
enum SSVM_HostRegistration
#ifdef __cplusplus
    : uint8_t
#endif // __cplusplus
{ SSVM_HostRegistration_Wasi = 0,
  SSVM_HostRegistration_SSVM_Process,
  SSVM_HostRegistration_Max };

/// Opaque struct of SSVM configure.
typedef struct {
} SSVM_ConfigureContext;

#ifdef __cplusplus
extern "C" {
#endif

/// ======== SSVM version functions ============================================

/// Get the version string of the SSVM C API.
///
/// The returned string must NOT be freed.
///
/// \returns C string of version with null terminated.
const char *SSVM_VersionGet();

/// Get the major version value of the SSVM C API.
///
/// \returns Value of the major version.
uint32_t SSVM_VersionGetMajor();

/// Get the minor version value of the SSVM C API.
///
/// \returns Value of the minor version.
uint32_t SSVM_VersionGetMinor();

/// Get the patch version value of the SSVM C API.
///
/// \returns Value of the patch version.
uint32_t SSVM_VersionGetPatch();

/// ======== SSVM version functions ============================================

/// ======== SSVM result functions =============================================

/// Check the result is success or not.
///
/// \param Res the SSVM_Result struct.
///
/// \returns true if the error code is ErrCode::Success or ErrCode::Terminated,
/// false for others.
bool SSVM_ResultOK(SSVM_Result Res);

/// Get the result code.
///
/// \param Res the SSVM_Result struct.
///
/// \returns corresponding result code.
uint32_t SSVM_ResultGetCode(SSVM_Result Res);

/// Get the result message.
///
/// The returned string must NOT be freed.
///
/// \param Res the SSVM_Result struct.
///
/// \returns C string of the correspondig message with null terminated.
const char *SSVM_ResultGetMessage(SSVM_Result Res);

/// ======== SSVM result functions =============================================

/// ======== SSVM configure functions ==========================================

/// Creation of SSVM_ConfigureContext.
///
/// The caller owns the object and should call `SSVM_ConfigureDelete` to free
/// it.
///
/// \returns pointer to context, null if failed.
SSVM_ConfigureContext *SSVM_ConfigureCreate();

/// Add a proposal setting into SSVM_ConfigureContext.
///
/// For turning on a specific WASM proposal in SSVM_VMContext, you can set the
/// proposal into SSVM_ConfigureContext and create VM with this context.
/// ```c
/// SSVM_ConfigureContext *Conf = SSVM_ConfigureCreate();
/// SSVM_ConfigureAddProposal(Conf, SSVM_Proposal_BulkMemoryOperations);
/// SSVM_ConfigureAddProposal(Conf, SSVM_Proposal_ReferenceTypes);
/// SSVM_ConfigureAddProposal(Conf, SSVM_Proposal_SIMD);
/// SSVM_VMContext *VM = SSVM_VMCreate(Conf);
/// ```
///
/// \param Cxt the SSVM_ConfigureContext to add the proposal.
/// \param Prop the proposal value.
void SSVM_ConfigureAddProposal(SSVM_ConfigureContext *Cxt, SSVM_Proposal Prop);

/// Remove a proposal setting in SSVM_ConfigureContext.
///
/// \param Cxt the SSVM_ConfigureContext to remove the proposal.
/// \param Prop the proposal value.
void SSVM_ConfigureRemoveProposal(SSVM_ConfigureContext *Cxt,
                                  SSVM_Proposal Prop);

/// Check a proposal setting exists in SSVM_ConfigureContext or not.
///
/// \param Cxt the SSVM_ConfigureContext to check the proposal.
/// \param Prop the proposal value.
///
/// \returns true if the proposal setting exists, false if not.
bool SSVM_ConfigureHasProposal(SSVM_ConfigureContext *Cxt, SSVM_Proposal Prop);

/// Add a host pre-registration setting into SSVM_ConfigureContext.
///
/// For turning on the Wasi support in SSVM_VMContext, you can set the host
/// pre-registration into SSVM_ConfigureContext and create VM with this context.
/// ```c
/// SSVM_ConfigureContext *Conf = SSVM_ConfigureCreate();
/// SSVM_ConfigureAddHostRegistration(Conf, SSVM_HostRegistration_Wasi);
/// SSVM_VMContext *VM = SSVM_VMCreate(Conf);
/// ```
///
/// \param Cxt the SSVM_ConfigureContext to add host pre-registration.
/// \param Host the host pre-registration value.
void SSVM_ConfigureAddHostRegistration(SSVM_ConfigureContext *Cxt,
                                       SSVM_HostRegistration Host);

/// Remove a host pre-registration setting in SSVM_ConfigureContext.
///
/// \param Cxt the SSVM_ConfigureContext to remove the host pre-registration.
/// \param Prop the host pre-registration value.
void SSVM_ConfigureRemoveHostRegistration(SSVM_ConfigureContext *Cxt,
                                          SSVM_HostRegistration Host);

/// Check a host pre-registration setting exists in SSVM_ConfigureContext or
/// not.
///
/// \param Cxt the SSVM_ConfigureContext to check the host pre-registration.
/// \param Prop the host pre-registration value.
///
/// \returns true if the host pre-registration setting exists, false if not.
bool SSVM_ConfigureHasHostRegistration(SSVM_ConfigureContext *Cxt,
                                       SSVM_HostRegistration Host);

void SSVM_ConfigureSetMaxMemoryPage(SSVM_ConfigureContext *Cxt,
                                    const uint32_t Page);

const uint32_t SSVM_ConfigureGetMaxMemoryPage(SSVM_ConfigureContext *Cxt);

/// Deletion of SSVM_ConfigureContext.
///
/// After calling this function, the context will be freed and should NOT be
/// used.
///
/// \param Cxt the SSVM_ConfigureContext to delete.
void SSVM_ConfigureDelete(SSVM_ConfigureContext *Cxt);

/// ======== SSVM configure functions ==========================================

#ifdef __cplusplus
} /// extern "C"
#endif

#endif /// __SSVM_C_API_H__